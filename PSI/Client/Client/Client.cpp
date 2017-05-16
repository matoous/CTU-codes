/*******************************************************************
PSI semestral project by Matous Dzivjak
(C) Copyright 2017 - 2017 by Matous Dzivjak
e-mail:   dzivjmat@fel.cvut.cz
license:  MIT
Feel free to copy as long as you wont use it for the homework
*******************************************************************/

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

/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78
// time to wait for packet ACK
#define SOCKET_READ_TIMEOUT_SEC 1
// sliding window size
#define SLIDING_WINDOW_SIZE 4
// max resend attempts
#define MAX_RESEND_ATTEMPTS 5

/* NAMESPACE */
using namespace std;

// Frame struct, holds info about data offset, send attempts, time of first send (I do not use this one), and if the data were succesfuly received
typedef struct frame_t {
	uint32_t data_offset;
	chrono::time_point<chrono::system_clock> time;
	bool ack;
	unsigned char attempts;
} frame_t;

// Window struct, circular queue with bonuses
typedef struct window_t {
	uint16_t size;
	vector<frame_t> window;
	uint16_t front, end, elements;
} window_t;

// Init sliding window with given size
window_t initWindow(int s) {
	window_t w;
	w.size = s;
	w.window.resize(w.size);
	w.front = w.elements = 0;
	w.end = -1;
	return w;
}

// Add try to frame
bool windowAddTry(window_t* W, uint32_t noff) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == noff) {
			(*W).window[idx].attempts++;
			if ((*W).window[idx].attempts == MAX_RESEND_ATTEMPTS)
				return false;
		}
	}
	return true;
}

// Is window full?
bool windowFull(window_t* W) {
	return (*W).elements == (*W).size;
}

// Is window empty?
bool windowEmpty(window_t* W) {
	return (*W).elements == 0;
}

// Add frame to window
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

// Acknowledge given data and slide window accordingly
void windowAck(window_t* W, uint32_t off) {
	int idx = (*W).front;
	for (int u = 0; u < (*W).elements; u++) {
		if ((*W).window[idx].data_offset == off) {
			(*W).window[idx].ack = true;
			printf("Successfuly ACK frame with offset %u index %d in the window\n", off, idx);
			if (idx == (*W).front) {
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

// CRC function
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
	char buffer[4096];
	ZeroMemory(buffer, sizeof(buffer));

	// print string to buffer
	for (int i = 0; i < 4092; i++)
		buffer[i] = s[i];

	// compute crc
	uint32_t crc = 0;
	crc = crc32c(crc, buffer, 4092);
	vector<unsigned char> crc_bytes = intToBytes(crc);
	for (int i = 4092; i < 4096; i++)
		buffer[i] = crc_bytes[i - 4092];

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
	for (int i = 4092; i < 4096; i++)
		buffer[i] = crc_bytes[i-4092];

	// set select
	fd_set set;
	struct timeval timeout;
	timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC;
	timeout.tv_usec = 0;

	// try sanding, max five times
	do {
		tries++;
		if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
		{
			FD_ZERO(&set); /* clear the set */
			FD_SET(socketC, &set); /* add our file descriptor to the set */
			int rv = select(socketC, &set, NULL, NULL, &timeout);
			if (rv == SOCKET_ERROR) // select error
				return false;
			else if (rv == 0) // timeout
				continue;
			else // received in time
				if (recvfrom(socketC, back_buff, sizeof(back_buff), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
					printf("Receive response from server: %s after %d tries\n", back_buff, tries);
		}
	} while (strncmp(back_buff, "OK", 2) != 0 && tries != MAX_RESEND_ATTEMPTS);

	return tries < MAX_RESEND_ATTEMPTS;
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
	for (int i = 0; i < hashed.size(); i++)
		str[i + 5] = hashed[i];
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
	window_t W = initWindow(SLIDING_WINDOW_SIZE);
	char back_buff[16];

	// set select
	fd_set set;
	struct timeval timeout;
	timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC;
	timeout.tv_usec = 0;

	while (offset < file_bytes.size() || !windowEmpty(&W)) {
		if (!windowFull(&W) && offset < file_bytes.size()) {
			// create data packet
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

			// add frame to the window
			frame_t nf;
			nf.ack = false;
			nf.data_offset = offset;
			nf.time = chrono::system_clock::now();
			nf.attempts = 1;
			windowAdd(&W, nf);

			// increment offset
			offset += 4096 - 12;

			// clean buffer
			ZeroMemory(buffer2, sizeof(buffer2));
		}
		else {
			FD_ZERO(&set); /* clear the set */
			FD_SET(socketC, &set); /* add our file descriptor to the set */
			int rv = select(socketC, &set, NULL, NULL, &timeout);
			if (rv == SOCKET_ERROR) // select error
				printf("Select error\n");
			else if (rv == 0){ // Timeout, resend all data that were not ACK in specified time window
				chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
				chrono::duration<double> elapsed_second;
				int idx = W.front;
				for (int u = 0; u < W.elements; u++) {
					elapsed_second = now - W.window[idx].time;
					if (elapsed_second.count() > 1) { // check if one second passed
						W.window[idx].attempts++; // add try
						W.window[idx].time = now; // set tu current time
						// setup data packet
						buffer2[0] = 'D';
						buffer2[1] = 'A';
						buffer2[2] = 'T';
						buffer2[3] = 'A';
						// offset
						vector<unsigned char> offset_bytes = intToBytes(W.window[idx].data_offset);
						for (int i = 0; i < 4; i++)
							buffer2[i + 4] = offset_bytes[i];
						for (int i = 0; i < ((4096 - 12 < file_bytes.size() - W.window[idx].data_offset) ? (4096 - 12) : (file_bytes.size() - W.window[idx].data_offset)); i++)
							buffer2[i + 8] = file_bytes[i + W.window[idx].data_offset];
						if (!sendData(buffer2)) {
							cout << "Error sending DATA packet" << endl;
							return 0;
						}
					}
					idx++;
					if (idx == W.size)
						idx = 0;
				}
			}
			else{ // Received in time
				if (recvfrom(socketC, back_buff, sizeof(back_buff), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR) {
					if (strncmp(back_buff, "ACK", 3) == 0) { // Packet acknowladgment
						uint32_t noff = 0;
						for (int n = 0; n < 4; n++)
							noff = (noff << 8) + (unsigned char)back_buff[n + 3];
						windowAck(&W, noff);
					}
					else if (strncmp(back_buff, "RFS", 3) == 0) { // Packet received but deformed
						// read packet offset
						uint32_t noff = 0;
						for (int n = 0; n < 4; n++)
							noff = (noff << 8) + (unsigned char)back_buff[n + 3];
						printf("Received RFS for packet with offset %u\n", noff);
						if (windowAddTry(&W, noff)) { // try to send again, fail if already failed few times
							buffer2[0] = 'D';
							buffer2[1] = 'A';
							buffer2[2] = 'T';
							buffer2[3] = 'A';
							// offset
							vector<unsigned char> offset_bytes = intToBytes(noff);
							for (int i = 0; i < 4; i++)
								buffer2[i + 4] = offset_bytes[i];
							// data
							for (int i = 0; i < ((4096 - 12 < file_bytes.size() - noff) ? (4096 - 12) : (file_bytes.size() - noff)); i++)
								buffer2[i + 8] = file_bytes[i + noff];
							if (!sendData(buffer2)) {
								cout << "Error sending DATA packet" << endl;
								return 0;
							}
						}
						else { // already tried too many times
							printf("%d times attempted to resend data with offset %u, ending...\n", MAX_RESEND_ATTEMPTS, noff);
							exit(1);
						}
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

	// close
	system("pause");
}
