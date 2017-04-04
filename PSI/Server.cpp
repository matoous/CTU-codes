#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

void InitWinsock(){
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
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

	// loop
	while (1)
	{
		char buffer[4096];
		ZeroMemory(buffer, sizeof(buffer));
		if (recvfrom(socketS, buffer, sizeof(buffer), 0, (sockaddr*)&from, &fromlen) != SOCKET_ERROR)
		{
			if (strncmp(buffer, "NAME", 4) == 0) {
				printf("Received NAME packet\n");
				int l = 0;
				for (int i = 5; i < 1000; i++) {
					file_name[l++] = buffer[i];
					if (buffer[i] == '\0') break;
				}
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
			}
			else if (strncmp(buffer, "START", 5) == 0) {
				printf("Received START packet\n");
				incomming_bytes = (char*)malloc(file_size*sizeof(char));
			}
			else if (strncmp(buffer, "DATA", 4) == 0) {
				uint32_t offset = 0;
				for (int n = 0; n < 4; n++)
					offset = (offset << 8) + (unsigned char)buffer[n + 4];
				if (offset + 4096 > file_size) {
					for (int i = 0; i < file_size - offset; i++) {
						incomming_bytes[i + offset] = buffer[i+8];
					}
				}
				else {
					for (int i = 0; i < 4096 - 8; i++) {
						incomming_bytes[i + offset] = buffer[i+8];
					}
				}
				printf("Received DATA packet with offset: %u\n", offset);
			}
			else if (strncmp(buffer, "STOP", 4) == 0) {
				printf("Received STOP packet.\n");
				printf("Writing file.");
				ofstream outfile(file_name, ofstream::binary);
				outfile.write(incomming_bytes, file_size);
				free(incomming_bytes);
			}
			sendto(socketS, buffer, 8, 0, (sockaddr*)&from, fromlen);
		}
	}
	closesocket(socketS);

	return 0;
}

