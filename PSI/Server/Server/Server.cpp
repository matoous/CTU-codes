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
#include <stdint.h>
#include <Windows.h>
#include <cstdlib>
#include "md5.h"

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

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

void InitWinsock(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/* convert int to bytes */
vector<unsigned char> intToBytes(uint32_t paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// SETUP, do not tuch!
	SOCKET socketS;

	InitWinsock();
	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	local.sin_family = AF_INET;
	local.sin_port = htons(4000);
	local.sin_addr.s_addr = INADDR_ANY;

	socketS = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socketS, (sockaddr*)&local, sizeof(local));
	printf("Listening on port 4000\n");

	// my vars
	uint32_t file_size;
	char file_name[64];
	char* incomming_bytes;
	char buffer[4096];
	char file_hash[128];
	char* re_message;
	int fails_in_row = 0;

	// loop
	while (1)
	{
		ZeroMemory(buffer, sizeof(buffer));
		if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
		{
			// Check CRC
			uint32_t check_crc = 0;
			for (int n = 4092; n < 4096; n++)
				check_crc = (check_crc << 8) + (unsigned char)buffer[n];
			uint32_t crc = 0;
			crc = crc32c(crc, buffer, 4096 - 4);
			if (check_crc != crc) {
				fails_in_row++;
				if (fails_in_row == 8) {
					cout << "transition failed" << endl;
				}
				if (strncmp(buffer, "DATA", 4) == 0) {
					char* bb = new char[16];
					bb[0] = 'R';
					bb[1] = 'F';
					bb[2] = 'S';
					for (int i = 0; i < 4; i++)
						bb[i + 3] = buffer[i + 4];
				}
				else {
					sendto(socketS, "BAD_____________", 16, 0, (sockaddr*)&from, fromlen);
				}
				continue;
			}
			else {
				fails_in_row = 0;
				if (strncmp(buffer, "NAME", 4) == 0) {
					printf("Received NAME packet\n");
					int l = 0;
					for (int i = 5; i < 1000; i++) {
						file_name[l++] = buffer[i];
						if (buffer[i] == '\0') break;
					}
					re_message = "OK______________";
				}
				else if (strncmp(buffer, "SIZE", 4) == 0) {
					printf("Received SIZE packet\n");
					file_size = 0;
					int i = 5;
					while (1) {
						if (buffer[i] == '\0') break;
						file_size *= 10;
						file_size += buffer[i++] - '0';
					}
					re_message = "OK______________";
				}
				else if (strncmp(buffer, "START", 5) == 0) {
					printf("Received START packet\n");
					incomming_bytes = (char*)malloc(file_size * sizeof(char));
				}
				else if (strncmp(buffer, "DATA", 4) == 0) {
					uint32_t offset = 0;
					for (int n = 0; n < 4; n++)
						offset = (offset << 8) + (unsigned char)buffer[n + 4];
					if (offset + 4096 > file_size) {
						for (int i = 0; i < file_size - offset; i++) {
							incomming_bytes[i + offset] = buffer[i + 8];
						}
					}
					else {
						for (int i = 0; i < 4096 - 12; i++) {
							incomming_bytes[i + offset] = buffer[i + 8];
						}
					}
					// TESTING
					int rnd = rand() % 100;
					if (rnd < 20) {
						vector<unsigned char> offbytes = intToBytes(offset);
						re_message = new char[16];
						re_message[0] = 'R';
						re_message[1] = 'F';
						re_message[2] = 'S';
						for (int i = 0; i < 4; i++)
							re_message[i + 3] = offbytes[i];
						cout << "sending RFS for " << offset << endl;
					}
					else {
						vector<unsigned char> offbytes = intToBytes(offset);
						re_message = new char[16];
						re_message[0] = 'A';
						re_message[1] = 'C';
						re_message[2] = 'K';
						for (int i = 0; i < 4; i++)
							re_message[i + 3] = offbytes[i];
						cout << "sending ACK for " << offset << endl;
					}
					// END TESTING
					/*
					vector<unsigned char> offbytes = intToBytes(offset);
					re_message = new char[16];
					re_message[0] = 'A';
					re_message[1] = 'C';
					re_message[2] = 'K';
					for (int i = 0; i < 4; i++)
					re_message[i + 3] = offbytes[i];
					cout << "sending ACK for " << offset << endl;
					*/
				}
				else if (strncmp(buffer, "HASH", 4) == 0) {
					printf("Received HASH packet\n");
					int l = 0;
					for (int i = 5; i < 128+5; i++) {
						file_hash[l++] = buffer[i];
						if (buffer[i] == '\0') break;
					}				
					re_message = "OK______________";
				}
				else if (strncmp(buffer, "STOP", 4) == 0) {
					printf("Received STOP packet.\n");
					printf("Writing file.\n");
					ofstream outfile(file_name, ofstream::binary);
					outfile.write(incomming_bytes, file_size);
					free(incomming_bytes);
					outfile.close();

					// file md5
					ifstream t(file_name);
					string str2;
					t.seekg(0, ios::end);
					str2.reserve(t.tellg());
					t.seekg(0, ios::beg);

					str2.assign((istreambuf_iterator<char>(t)),
						istreambuf_iterator<char>());
					string hashed = md5(str2);
					string orig_hash(file_hash);
					if (hashed.compare(orig_hash) == 0) {
						cout << "file ok" << endl;
						re_message = "OK______________";
					}
					else {
						cout << "file bad" << endl;
						re_message = "BF______________";
					}
				}
				sendto(socketS, re_message, 16, 0, (sockaddr*)&from, fromlen);
			}
		}
	}
	closesocket(socketS);
	return 0;
}

