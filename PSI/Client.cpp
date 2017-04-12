#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stddef.h>
#include <stdint.h>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

/* NAMESPACE */
using namespace std;

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

/* converti int to bytes */
vector<unsigned char> intToBytes(uint32_t paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

/* send string */
bool sendBuffer(const char* s) {
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
			}
		}
	} while (strncmp(back_buff, "OK", 2) != 0 && tries != 8);

	// return
	return tries < 8;
}

// By Matous Dzivjak (dzivjmat@fel.cvut.cz), take a look, write your own, don't be lame
int _tmain(int argc, _TCHAR* argv[])
{

	// setup
	InitWinsock();
	len = sizeof(serverInfo);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(4000);
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");                                                                    // CHANGE THIS
	string file = "C:\\Users\\Matouš\\Desktop\\vit_normal.ppm";                                                             // CHANGE THIS
	string fname = "out.ppm";                                                                                               // CHANGE THIS

	// open socket
	socketC = socket(AF_INET, SOCK_DGRAM, 0);

	// read file bytes
	vector<char> file_bytes = ReadAllBytes(file);

	char str[120];

	// send file name
	ZeroMemory(str, sizeof(str));
	sprintf(str, "NAME=%s", fname.c_str());
	if (!sendBuffer(str)) {
		cout << "Error sending NAME packet" << endl;
		return 0;
	}

	// send file size
	ZeroMemory(str, sizeof(str));
	sprintf(str, "SIZE=%ld", file_bytes.size());
	if (!sendBuffer(str)) {
		cout << "Error sending SIZE packet" << endl;
		return 0;
	}

	// start file transmit
	ZeroMemory(str, sizeof(str));
	sprintf(str, "START");
	if (!sendBuffer(str)) {
		cout << "Error sending START packet" << endl;
		return 0;
	}

	// transmit
	uint32_t offset = 0;
	char buffer2[4096];
	while (offset < file_bytes.size()) {
		buffer2[0] = 'D';
		buffer2[1] = 'A';
		buffer2[2] = 'T';
		buffer2[3] = 'A';
		// offset
		vector<unsigned char> offset_bytes = intToBytes(offset);
		for (int i = 0; i < 4; i++) {
			buffer2[i+4] = offset_bytes[i];
		}
		// data
		for (int i = 0; i < ((4096 - 12 < file_bytes.size() - offset) ? (4096-12) : (file_bytes.size() - offset)); i++) {
			buffer2[i+8] = file_bytes[i + offset];
		}
		// CRC
		uint32_t crc = 0;
		crc = crc32c(crc, buffer2, 4092);
		vector<unsigned char> crc_bytes = intToBytes(crc);
		for (int i = 4092; i < 4096; i++) {
			buffer2[i] = crc_bytes[i - 4092];
		}
		// send
		if (sendto(socketC, buffer2, sizeof(buffer2), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
		{
			if (recvfrom(socketC, buffer2, sizeof(buffer2), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
			{
				buffer2[15] = '\0';
				printf("Receive response from server: %s\n", buffer2);
			}
		}
		offset += 4096 - 12;
		ZeroMemory(buffer2, sizeof(buffer2));
	}

	// end file transmit
	ZeroMemory(str, sizeof(str));
	sprintf(str, "STOP");
	if (!sendBuffer(str)) {
		cout << "Error sending STOP packet" << endl;
		return 0;
	}

	// close socket
	char _;
	cin >> _;
	return 0;
}
