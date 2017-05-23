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
#include "rsa.h"
#include "utils.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

// max fails
#define MAX_FAILS_IN_ROW 10

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	// SETUP, do not tuch!
	SOCKET socketS;

	// init
	InitWinsock();
	struct sockaddr_in local;
	struct sockaddr_in from;
	int fromlen = sizeof(from);
	local.sin_family = AF_INET;
	local.sin_port = htons(4000);
	local.sin_addr.s_addr = INADDR_ANY;

	socketS = socket(AF_INET, SOCK_DGRAM, 0);
	bind(socketS, (sockaddr*)&local, sizeof(local));
	printf("INFO Listening on port 4000\n");
	cout << "Enter the handshake number: ";
	bignum_t hsn;
	cin >> hsn;

	// my vars
	Rsa rsa(743, 547);   // Maybe read as params
	uint32_t file_size;
	char file_name[64];
	char* incomming_bytes;
	char buffer[4096];
	char re_message[32];
	char file_hash[128];
	int fails_in_row = 0;

	// loop
	while (1)
	{
		ZeroMemory(buffer, sizeof(buffer));
		// wait for receive
		if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
		{
			// Check CRC
			uint32_t check_crc = 0;
			for (int n = 4092; n < 4096; n++)
				check_crc = (check_crc << 8) + (unsigned char)buffer[n];
			uint32_t crc = 0;
			crc = crc32c(crc, buffer, 4096 - 4);
			
			// crc bad
			if (check_crc != crc) {
				fails_in_row++;
				if (fails_in_row == MAX_FAILS_IN_ROW) // if failed too many times
					cerr << "ERROR transition failed" << endl;
				if (strncmp(buffer, "DATA", 4) == 0) { // if data packet and bad crc, send REFUSE packet with DATA OFFSET
					char* bb = new char[16];
					bb[0] = 'R';
					bb[1] = 'F';
					bb[2] = 'S';
					for (int i = 0; i < 4; i++)
						bb[i + 3] = buffer[i + 4];
				}
				else
					sendto(socketS, "BAD_____________________________", 32, 0, (sockaddr*)&from, fromlen);
				continue;
			}
			else {
				// check RSA
				bool rsa_ok = true;
				if (rsa.set) {
					MD5 md5(std::string(&buffer[0], &buffer[4096 - 4 - 128]));
					uint8_t* hashes = md5.digested();
					for (int i = 0; i < 16; i++) {
						bignum_t curr = 0;
						for (int u = 0; u < 8; u++)
							curr = (curr << 8) + (unsigned char)buffer[4096 - 4 - 128 + (i * 8) + u];
						if (hashes[i] != rsa.decrypt(curr)) {
							rsa_ok = false;
							cerr << "ERROR authentication error" << endl;
							break;
						}
					}
				}
				if (rsa_ok) {

					fails_in_row = 0;
					if (strncmp(buffer, "NAME", 4) == 0) { // NAME PACKET
						cout << "INFO Received NAME packet" << endl;
						int l = 0;
						for (int i = 5; i < 1000; i++) {
							file_name[l++] = buffer[i];
							if (buffer[i] == '\0') break;
						}
						sprintf(re_message, "OK____________________________");
					}
					else if (strncmp(buffer, "KEYS", 4) == 0) { // KEYS PACKET
						cout << "INFO Received KEYS packet" << endl;
						bignum_t n = 0;
						for (int i = 0; i < 8; i++)
							n = (n << 8) + (unsigned char)buffer[i + 5];
						bignum_t e = 0;
						for (int i = 0; i < 8; i++)
							e = (e << 8) + (unsigned char)buffer[i + 13];
						rsa.set_public(n, e);
						sprintf(re_message, "OKEY=");
						vector<unsigned char> nbytes = bigintToBytes(rsa.getN());
						for (int i = 0; i < 8; i++)
							re_message[i + 5] = nbytes[i];
						vector<unsigned char> ebytes = bigintToBytes(rsa.getE());
						for (int i = 0; i < 8; i++)
							re_message[i + 13] = ebytes[i];
					}
					else if (strncmp(buffer, "WELC", 4) == 0) { // KEYS PACKET
						cout << "INFO Received WELC packet" << endl;
						bignum_t msg = 0;
						for (int i = 0; i < 8; i++)
							msg = (msg << 8) + (unsigned char)buffer[i + 5];
						if (rsa.decrypt(msg) == hsn) {
							cout << "INFO the welcome message from client matches server side welcome message, replying." << endl;
							sprintf(re_message, "WELC=");
							vector<unsigned char> nbytes = bigintToBytes(rsa.encrypt(hsn));
							for (int i = 0; i < 8; i++)
								re_message[i + 5] = nbytes[i];
						}
						else {
							cerr << "ERROR received bad welcome message, received " << rsa.decrypt(msg) << " encrypted as " << msg << " should be " << hsn << endl;
							sprintf(re_message, "NWELC__________________________");
						}
					}
					else if (strncmp(buffer, "SIZE", 4) == 0) { // SIZE PACKET
						cout << "INFO Received SIZE packet" << endl;
						file_size = 0;
						int i = 5;
						while (1) {
							if (buffer[i] == '\0') break;
							file_size *= 10;
							file_size += buffer[i++] - '0';
						}
						cout << "FILE size " << file_size << endl;
						sprintf(re_message, "OK____________________________");
					}
					else if (strncmp(buffer, "START", 5) == 0) { // START PACKET
						cout << "INFO Received START packet" << endl;
						incomming_bytes = (char*)malloc(file_size * sizeof(char));
					}
					else if (strncmp(buffer, "DATA", 4) == 0) { // DATA PACKET
						uint32_t offset = 0;
						for (int n = 0; n < 4; n++)
							offset = (offset << 8) + (unsigned char)buffer[n + 4];
						if (offset + 4096 - 128 - 12 > file_size)
							for (int i = 0; i < file_size - offset; i++)
								incomming_bytes[i + offset] = buffer[i + 8];
						else
							for (int i = 0; i < 4096 - 12 - 128; i++)
								incomming_bytes[i + offset] = buffer[i + 8];
						// return ACK for given data on given offset
						vector<unsigned char> offbytes = intToBytes(offset);
						re_message[0] = 'A';
						re_message[1] = 'C';
						re_message[2] = 'K';
						for (int i = 0; i < 4; i++)
							re_message[i + 3] = offbytes[i];
						cout << "INFO sending ACK for " << offset << endl;
					}
					else if (strncmp(buffer, "HASH", 4) == 0) { // HASH PACKET
						cout << "INFO received HASH packet ";
						int l = 0;
						for (int i = 5; i < 128 + 5; i++) {
							file_hash[l++] = buffer[i];
							if (buffer[i] == '\0') break;
						}
						cout << file_hash << endl;
						sprintf(re_message, "OK____________________________");
					}
					else if (strncmp(buffer, "STOP", 4) == 0) { // STOP PACKET
						cout << "INFO Received STOP packet. Writing file." << endl;

						//cout << string(incomming_bytes) << endl;

						// write file
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

						// check file HASH
						if (hashed.compare(orig_hash) == 0) {
							cout << "INFO file is ok, saved" << endl;
							sprintf(re_message, "OK____________________________");
						}
						else {
							cerr << "ERROR file hash does not match the original one." << endl;
							sprintf(re_message, "BF____________________________");
						}
					}
					// send response
					sendto(socketS, re_message, 32, 0, (sockaddr*)&from, fromlen);
				}
			}
		}
	}
	// close socket
	closesocket(socketS);
	return 0;
}

