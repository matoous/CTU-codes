#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stddef.h>
#include <chrono>
#include <thread>
#include <stdint.h>
#include "md5.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

/* NAMESPACE */
using namespace std;

typedef struct frame_t {
	uint32_t data_offset;
	chrono::time_point<chrono::system_clock> time;
	bool ack;
	unsigned char attempts;
} frame_t;

typedef struct window_t {
	uint16_t size;
	vector<frame_t> window;
	uint16_t front, end, elements;
} window_t;

window_t initWindow(int s) {
	window_t w;
	w.size = s;
	w.window.resize(w.size);
	w.front = w.elements = 0;
	w.end = -1;
	return w;
}

bool windowAddTry(window_t* W, uint32_t noff) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == noff) {
			(*W).window[idx].attempts++;
			if ((*W).window[idx].attempts == 8)
				return false;
		}
	}
	return true;
}

bool windowFull(window_t* W) {
	return (*W).elements == (*W).size;
}

bool windowEmpty(window_t* W) {
	return (*W).elements == 0;
}

bool windowAdd(window_t* W, frame_t F) {
	if (windowFull(W))
		return false;
	else {
		(*W).end++;
		if ((*W).end == (*W).size)
			(*W).end = 0;
		(*W).window[(*W).end] = F;
		(*W).elements++;
	}
}

void windowAck(window_t* W, uint32_t off) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == off) {
			(*W).window[idx].ack = true;
			printf("Successfuly ACK frame with offset %u index %d in the window\n", off, idx);
			if (idx == (*W).front) {
				printf("Removing from Window\n");
				while ((*W).elements != 0 && (*W).window[(*W).front].ack) {
					(*W).front++;
					if ((*W).front == (*W).size)
						(*W).front = 0;
					(*W).elements--;
				}
			}
		}
		idx++;
		if (idx == (*W).size)
			idx = 0;
	}
}

unsigned int sleep_time = 0;

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

/* CRC-32 (Ethernet, ZIP, etc.) polynomial in reversed bit order. */
/* #define POLY 0xedb88320 */
uint32_t crc32c(uint32_t crc, const char *buf, size_t len)
{
	int k;

	crc = ~crc;
	while (len--) {
		crc ^= *buf++;
		for (k = 0; k < 8; k++)
			crc = crc & 1 ? (crc >> 1) ^ POLY : crc >> 1;
	}
	return ~crc;
}
/* CRC END */

/* Global variables */
SOCKET socketC;
struct sockaddr_in serverInfo;
int len;

/* Socket init */
void InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/* Read all bytes from file */
static vector<char> ReadAllBytes(const string filename)
{
	ifstream ifs(filename, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	vector<char>  result(pos);
	char* result2 = (char*)malloc(pos * sizeof(char));

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);

	ifs.seekg(0, ios::beg);
	ifs.read(result2, pos);

	return result;
}

/* convert int to bytes */
vector<unsigned char> intToBytes(uint32_t paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

bool sendData(const char* s) {
	// init variables
	int tries = 0;
	char back_buff[16];
	char buffer[4096];
	ZeroMemory(buffer, sizeof(buffer));

	// print string to buffer
	for (int i = 0; i < 4092; i++)
		buffer[i] = s[i];

	// compute crc
	uint32_t crc = 0;
	crc = crc32c(crc, buffer, 4092);
	vector<unsigned char> crc_bytes = intToBytes(crc);
	for (int i = 4092; i < 4096; i++) {
		buffer[i] = crc_bytes[i - 4092];
	}

	return sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR;
}


/* send string */
bool sendString(const char* s) {
	// init variables
	int tries = 0;
	char back_buff[16];
	char buffer[4096];
	ZeroMemory(buffer, sizeof(buffer));

	// print string to buffer
	sprintf(buffer, s);
	
	// compute crc
	uint32_t crc = 0;
	crc = crc32c(crc, buffer, 4092);
	vector<unsigned char> crc_bytes = intToBytes(crc);
	for (int i = 4092; i < 4096; i++) {
		buffer[i] = crc_bytes[i-4092];
	}

	// try sanding, max five times
	do {
		tries++;
		if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
		{
			if (recvfrom(socketC, back_buff, sizeof(back_buff), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
			{
				back_buff[15] = '\0';
				printf("Receive response from server: %s\n", back_buff);
				// adjust speed if needed
				if (strncmp(back_buff, "OKSS", 4) == 0) {
					uint32_t bps = 0;
					for (int n = 0; n < 4; n++)
						bps = (bps << 8) + (unsigned char)back_buff[n + 4];
					sleep_time = round(((float)4096 / bps) * 1000);
				}
			}
		}
	} while (strncmp(back_buff, "OK", 2) != 0 && tries != 8);

	// return
	return tries < 8;
}



int _tmain(int argc, _TCHAR* argv[])
{
	// setup
	InitWinsock();
	len = sizeof(serverInfo);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(4000);
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");                                    // CHANGE THIS
	string file = "D:\\in.txt";                                                             // CHANGE THIS
	string fname = "D:\\out.txt";                                                           // CHANGE THIS


	// open socket
	socketC = socket(AF_INET, SOCK_DGRAM, 0);

	// file md5
	std::ifstream t(file);
	std::string str2;

	t.seekg(0, std::ios::end);
	str2.reserve(t.tellg());
	t.seekg(0, std::ios::beg);

	str2.assign((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
	string hashed = md5(str2);

	// read file bytes
	vector<char> file_bytes = ReadAllBytes(file);

	char str[120];

	// send file name
	ZeroMemory(str, sizeof(str));
	sprintf(str, "NAME=%s", fname.c_str());
	if (!sendString(str)) {
		cout << "Error sending NAME packet" << endl;
		return 0;
	}

	// send file size
	ZeroMemory(str, sizeof(str));
	sprintf(str, "SIZE=%ld", file_bytes.size());
	cout << "file size " << file_bytes.size() << endl;
	if (!sendString(str)) {
		cout << "Error sending SIZE packet" << endl;
		return 0;
	}

	// send file hash
	ZeroMemory(str, sizeof(str));
	sprintf(str, "HASH=");
	for (int i = 0; i < hashed.size(); i++) {
		str[i + 5] = hashed[i];
	}
	str[5 + hashed.size()] = '\0';
	cout << "sanding hash " << str << endl;
	if (!sendString(str)) {
		cout << "Error sending HASH packet" << endl;
		return 0;
	}

	// start file transmit
	ZeroMemory(str, sizeof(str));
	sprintf(str, "START");
	if (!sendString(str)) {
		cout << "Error sending START packet" << endl;
		return 0;
	}

	// transmit
	uint32_t offset = 0;
	char buffer2[4096];
	window_t W = initWindow(4);
	char back_buff[16];

	while (offset < file_bytes.size() || !windowEmpty(&W)) {
		printf("Window ele: %u\n", W.elements);
		if (!windowFull(&W) && offset < file_bytes.size()) {
			buffer2[0] = 'D';
			buffer2[1] = 'A';
			buffer2[2] = 'T';
			buffer2[3] = 'A';
			// offset
			vector<unsigned char> offset_bytes = intToBytes(offset);
			for (int i = 0; i < 4; i++) {
				buffer2[i + 4] = offset_bytes[i];
			}
			// data
			for (int i = 0; i < ((4096 - 12 < file_bytes.size() - offset) ? (4096 - 12) : (file_bytes.size() - offset)); i++) {
				buffer2[i + 8] = file_bytes[i + offset];
			}
			if (!sendData(buffer2)) {
				cout << "Error sending DATA packet" << endl;
				return 0;
			}

			frame_t nf;
			nf.ack = false;
			nf.data_offset = offset;
			nf.time = chrono::system_clock::now();
			nf.attempts = 1;
			windowAdd(&W, nf);
			printf("Send data off %u\n", nf.data_offset);
			offset += 4096 - 12;
			ZeroMemory(buffer2, sizeof(buffer2));
		}
		else {
			printf("Waiting for ACK\n");
			if (recvfrom(socketC, back_buff, sizeof(back_buff), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR) {
				if (strncmp(back_buff, "ACK", 3) == 0) {
					uint32_t noff = 0;
					for (int n = 0; n < 4; n++)
						noff = (noff << 8) + (unsigned char)back_buff[n + 3];
					printf("Received ACK for packet with offset %u\n", noff);
					windowAck(&W, noff);
				}
				else if (strncmp(back_buff, "RFS", 3) == 0) {
					uint32_t noff = 0;
					for (int n = 0; n < 4; n++)
						noff = (noff << 8) + (unsigned char)back_buff[n + 3];
					printf("Received RFS for packet with offset %u\n", noff);
					if(windowAddTry(&W, noff)) {
						buffer2[0] = 'D';
						buffer2[1] = 'A';
						buffer2[2] = 'T';
						buffer2[3] = 'A';
						// offset
						vector<unsigned char> offset_bytes = intToBytes(noff);
						for (int i = 0; i < 4; i++) {
							buffer2[i + 4] = offset_bytes[i];
						}
						// data
						for (int i = 0; i < ((4096 - 12 < file_bytes.size() - noff) ? (4096 - 12) : (file_bytes.size() - noff)); i++) {
							buffer2[i + 8] = file_bytes[i + noff];
						}
						if (!sendData(buffer2)) {
							cout << "Error sending DATA packet" << endl;
							return 0;
						}
					}
					else {
						printf("8 times received RFS for %u, ending...\n", noff);
						exit(1);
					}
				}
			}
		}
	}

	// end file transmit
	ZeroMemory(str, sizeof(str));
	sprintf(str, "STOP");
	if (!sendString(str)) {
		cout << "Error sending STOP packet" << endl;
		return 0;
	}
	// close socket
	system("pause");
}
