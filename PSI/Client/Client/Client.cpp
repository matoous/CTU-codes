/*******************************************************************
project:         PSI semestral project by Matous Dzivjak
(C) Copyright:   2017 - 2017 by Matous Dzivjak
e-mail:          dzivjmat@fel.cvut.cz
license:         MIT
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
#include "rsa.h"
#include "crc.h"
#include "sliding_window.h"

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define SOCKET_READ_TIMEOUT_SEC 1
#define SLIDING_WINDOW_SIZE 4
#define DEFAULT_LEN 64
#define PACKET_SIZE 4096

/* NAMESPACE */
using namespace std;

/* Global variables */
SOCKET socketC;
struct sockaddr_in serverInfo;
int len;
Rsa rsa(911, 571);
bignum_t shn;

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

/* convert 64bit int to bytes */
vector<unsigned char> bigintToBytes(bignum_t paramInt)
{
	vector<unsigned char> arrayOfBytes(8);
	for (int i = 0; i < 8; i++)
		arrayOfBytes[7 - i] = (paramInt >> (i * 8));
	return arrayOfBytes;
}

bool sendData(const char* s) {
	// init variables
	char buffer[PACKET_SIZE];
	ZeroMemory(buffer, sizeof(buffer));

	// print string to buffer
	for (int i = 0; i < PACKET_SIZE - 4 - 128; i++)
		buffer[i] = s[i];

	// hash and encrypt
	MD5 md5(std::string(&buffer[0], &buffer[PACKET_SIZE - 4 - 128]));
	uint8_t* hashes = md5.digested();
	for (int i = 0; i < 16; i++) {
	    vector<unsigned char> bytes = bigintToBytes(rsa.encrypt((bignum_t)hashes[i]));
		for (int u = 0; u < 8; u++) {
			buffer[PACKET_SIZE - 4 - 128 + (i * 8) + u] = bytes[u];
		}
	}

	// compute crc
	uint32_t crc = 0;
	crc = crc32c(crc, buffer, PACKET_SIZE-4);
	vector<unsigned char> crc_bytes = intToBytes(crc);
	for (int i = PACKET_SIZE-4; i < PACKET_SIZE; i++)
		buffer[i] = crc_bytes[i - (PACKET_SIZE - 4)];

	return sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR;
}

/* send string */
bool sendString(const char* s) {
	// init variables
	int tries = 0;
	char back_buff[DEFAULT_LEN];
	char buffer[PACKET_SIZE];
	ZeroMemory(buffer, sizeof(buffer));

	// print string to buffer
	for (int i = 0; i < DEFAULT_LEN; i++)
		buffer[i] = s[i];

	// hash and encrypt
	MD5 md5(std::string(&buffer[0], &buffer[PACKET_SIZE - 4 - 128]));
	uint8_t* hashes = md5.digested();
	for (int i = 0; i < 16; i++) {
		vector<unsigned char> bytes = bigintToBytes(rsa.encrypt((bignum_t)hashes[i]));
		for (int u = 0; u < 8; u++) {
			buffer[PACKET_SIZE - 4 - 128 + (i * 8) + u] = bytes[u];
		}
	}
	
	// compute crc
	uint32_t crc = 0;
	crc = crc32c(crc, buffer, PACKET_SIZE-4);
	vector<unsigned char> crc_bytes = intToBytes(crc);
	for (int i = PACKET_SIZE-4; i < PACKET_SIZE; i++)
		buffer[i] = crc_bytes[i-(PACKET_SIZE-4)];

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
				if (recvfrom(socketC, back_buff, sizeof(back_buff), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR) {
					printf("Receive response from server: %s after %d tries\n", back_buff, tries);
					if (strncmp(back_buff, "OKEY", 4) == 0) {
						bignum_t n = 0;
						for (int i = 0; i < 8; i++)
							n = (n << 8) + (unsigned char)back_buff[i + 5];
						bignum_t e = 0;
						for (int i = 0; i < 8; i++)
							e = (e << 8) + (unsigned char)back_buff[i + 13];
						rsa.set_public(n, e);
					}
					else if (strncmp(back_buff, "WELC", 4) == 0) {
						bignum_t msg = 0;
						for (int i = 0; i < 8; i++)
							msg = (msg << 8) + (unsigned char)back_buff[i + 5];
						if (rsa.decrypt(msg) == shn) {
							cout << "INFO souccessfuly handshaked with server" << endl;
							back_buff[0] = 'O';
							back_buff[1] = 'K';
						}
						else {
							cerr << "ERROR server send wrong handshake code" << endl;
							return false;
						}
					}
					else if (strncmp(back_buff, "NWELC", 5) == 0) {
						cerr << "ERROR you enter wrong handshake code" << endl;
						return false;
					}
				}
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

	char str[64];

	// send keys
	ZeroMemory(str, sizeof(str));
	sprintf(str, "KEYS=");
	vector<unsigned char> bytesN = bigintToBytes(rsa.getN());
	for (int i = 0; i < 8; i++)
		str[i+5] = bytesN[i];
	vector<unsigned char> bytesE = bigintToBytes(rsa.getE());
	for (int i = 0; i < 8; i++)
		str[i+13] = bytesE[i];
	if (!sendString(str)) {
		cerr << "ERROR sending KEYS packet" << endl;
		return 0;
	}

	// send handshake
	cout << "Enter handshake num: ";
	cin >> shn;
	sprintf(str, "WELC=");
	vector<unsigned char> welcome_msg = bigintToBytes(rsa.encrypt(shn));
	for (int i = 0; i < 8; i++)
		str[i + 5] = welcome_msg[i];
	if (!sendString(str)) {
		cerr << "ERROR sending WELCOM packet with handshake num " << shn << " encrypted as "<< rsa.encrypt(shn) << endl;
		return 0;
	}

	// send file name
	ZeroMemory(str, sizeof(str));
	sprintf(str, "NAME=%s", fname.c_str());
	if (!sendString(str)) {
		cerr << "ERROR sending NAME packet" << endl;
		return 0;
	}

	// send file size
	ZeroMemory(str, sizeof(str));
	sprintf(str, "SIZE=%ld", file_bytes.size());
	cout << "file size " << file_bytes.size() << endl;
	if (!sendString(str)) {
		cerr << "ERROR sending SIZE packet" << endl;
		return 0;
	}

	// send file hash
	ZeroMemory(str, sizeof(str));
	sprintf(str, "HASH=");
	for (int i = 0; i < hashed.size(); i++)
		str[i + 5] = hashed[i];
	str[5 + hashed.size()] = '\0';
	cout << "INFO sanding hash " << str << endl;
	if (!sendString(str)) {
		cerr << "ERROR sending HASH packet" << endl;
		return 0;
	}

	// start file transmit
	ZeroMemory(str, sizeof(str));
	sprintf(str, "START");
	if (!sendString(str)) {
		cerr << "ERROR sending START packet" << endl;
		return 0;
	}

	// transmit
	uint32_t offset = 0;
	char buffer[PACKET_SIZE];
	window_t W = initWindow(SLIDING_WINDOW_SIZE);
	char back_buff[32];

	// set select
	fd_set set;
	struct timeval timeout;
	timeout.tv_sec = SOCKET_READ_TIMEOUT_SEC;
	timeout.tv_usec = 0;

	while (offset < file_bytes.size() || !windowEmpty(&W)) {
		if (!windowFull(&W) && offset < file_bytes.size()) {
			// create data packet
			buffer[0] = 'D';
			buffer[1] = 'A';
			buffer[2] = 'T';
			buffer[3] = 'A';
			// offset
			vector<unsigned char> offset_bytes = intToBytes(offset);
			for (int i = 0; i < 4; i++)
				buffer[i + 4] = offset_bytes[i];
			// data
			for (int i = 0; i < ((PACKET_SIZE - 12 - 128 < file_bytes.size() - offset) ? (PACKET_SIZE - 12 - 128) : (file_bytes.size() - offset)); i++)
				buffer[i + 8] = file_bytes[i + offset];
			if (!sendData(buffer)) {
				cerr << "ERROR sending DATA packet" << endl;
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
			offset = offset + PACKET_SIZE - 12 - 128;

			// clean buffer
			ZeroMemory(buffer, sizeof(buffer));
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
						buffer[0] = 'D';
						buffer[1] = 'A';
						buffer[2] = 'T';
						buffer[3] = 'A';
						// offset
						vector<unsigned char> offset_bytes = intToBytes(W.window[idx].data_offset);
						for (int i = 0; i < 4; i++)
							buffer[i + 4] = offset_bytes[i];
						for (int i = 0; i < ((4096 - 12 - 128 < file_bytes.size() - W.window[idx].data_offset) ? (4096 - 12 - 128) : (file_bytes.size() - W.window[idx].data_offset)); i++)
							buffer[i + 8] = file_bytes[i + W.window[idx].data_offset];
						if (!sendData(buffer)) {
							cerr << "ERROR sending DATA packet" << endl;
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
						cout << "Received RFS for packet with offset " << noff << endl;
						if (windowAddTry(&W, noff)) { // try to send again, fail if already failed few times
							buffer[0] = 'D';
							buffer[1] = 'A';
							buffer[2] = 'T';
							buffer[3] = 'A';
							// offset
							vector<unsigned char> offset_bytes = intToBytes(noff);
							for (int i = 0; i < 4; i++)
								buffer[i + 4] = offset_bytes[i];
							// data
							for (int i = 0; i < ((4096 - 12 - 128 < file_bytes.size() - noff) ? (4096 - 12 - 128) : (file_bytes.size() - noff)); i++)
								buffer[i + 8] = file_bytes[i + noff];
							if (!sendData(buffer)) {
								cerr << "ERROR sending DATA packet" << endl;
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
		cerr << "ERROR sending STOP packet" << endl;
		system("pause");
		return 0;
	}

	// close
	system("pause");
}