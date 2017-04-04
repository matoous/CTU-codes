#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <fstream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

using namespace std;

void InitWinsock()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

long GetFileSize(string filename)
{
	struct stat stat_buf;
	int rc = stat(filename.c_str(), &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

static vector<char> ReadAllBytes(const string filename)
{
	ifstream ifs(filename, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	vector<char>  result(pos);

	ifs.seekg(0, ios::beg);
	ifs.read(&result[0], pos);

	return result;
}

vector<unsigned char> intToBytes(uint32_t paramInt)
{
	vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

int _tmain(int argc, _TCHAR* argv[])
{
	// setup
	SOCKET socketC;

	InitWinsock();
	struct sockaddr_in serverInfo;
	int len = sizeof(serverInfo);
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(4000);
	serverInfo.sin_addr.s_addr = inet_addr("127.0.0.1");
	string file = "C:\\Users\\Matouš\\Desktop\\vit_small.ppm";
	string fname = "vit_small.ppm";

	// buffer
	char buffer[4096];

	// open socket
	socketC = socket(AF_INET, SOCK_DGRAM, 0);

	// read file bytes
	vector<char> file_bytes = ReadAllBytes(file);
	
	// send file name
	ZeroMemory(buffer, sizeof(buffer));
	sprintf(buffer, "NAME=%s", fname.c_str());
	if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
	{
		if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
		{
			printf("Receive response from server: %s\n", buffer);
		}
	}

	// send file size
	ZeroMemory(buffer, sizeof(buffer));
	sprintf(buffer, "SIZE=%ld", file_bytes.size());
	if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
	{
		if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
		{
			printf("Receive response from server: %s\n", buffer);
		}
	}

	// start file transmit
	ZeroMemory(buffer, sizeof(buffer));
	sprintf(buffer, "START");
	if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
	{
		if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
		{
			printf("Receive response from server: %s\n", buffer);
		}
	}

	// transmit
	uint32_t offset = 0;
	while (offset + 4096 - 8 < file_bytes.size()) {
		buffer[0] = 'D';
		buffer[1] = 'A';
		buffer[2] = 'T';
		buffer[3] = 'A';
		vector<unsigned char> offset_bytes = intToBytes(offset);
		for (int i = 0; i < 4; i++) {
			buffer[i+4] = offset_bytes[i];
		}
		for (int i = 0; i < 4096 - 8; i++) {
			buffer[i+8] = file_bytes[i + offset];
		}
		if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
		{
			if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
			{
				printf("Receive response from server: %s\n", buffer);
			}
		}
		offset += 4096 - 8;
		ZeroMemory(buffer, sizeof(buffer));
	}
	buffer[0] = 'D';
	buffer[1] = 'A';
	buffer[2] = 'T';
	buffer[3] = 'A';
	vector<unsigned char> offset_bytes = intToBytes(offset);
	for (int i = 0; i < 4; i++) {
		buffer[i + 4] = offset_bytes[i];
	}
	for (int i = 0; i < file_bytes.size() - offset; i++) {
		buffer[i + 8] = file_bytes[i + offset];
	}
	if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
	{
		if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
		{
			printf("Receive response from server: %s\n", buffer);
		}
	}

	// stop file transmit
	ZeroMemory(buffer, sizeof(buffer));
	sprintf(buffer, "STOP");
	if (sendto(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, len) != SOCKET_ERROR)
	{
		if (recvfrom(socketC, buffer, sizeof(buffer), 0, (sockaddr*)&serverInfo, &len) != SOCKET_ERROR)
		{
			printf("Receive response from server: %s\n", buffer);
		}
	}

	// close socket
	while (1) {
	
	};

	return 0;
}