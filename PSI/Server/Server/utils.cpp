#pragma once

#include "utils.h"

// init socket
void InitWinsock() {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

/* convert int to bytes */
std::vector<unsigned char> intToBytes(uint32_t paramInt)
{
	std::vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[3 - i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

/* convert 64bit int to bytes */
std::vector<unsigned char> bigintToBytes(int64_t paramInt)
{
	std::vector<unsigned char> arrayOfBytes(8);
	for (int i = 0; i < 8; i++)
		arrayOfBytes[7 - i] = (paramInt >> (i * 8));
	return arrayOfBytes;
}

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
