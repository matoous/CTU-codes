#pragma once

#include <stdint.h>
#include <Windows.h>
#include <vector>
#define POLY 0x82f63b78

// init socket
void InitWinsock();

/* convert int to bytes */
std::vector<unsigned char> intToBytes(uint32_t paramInt);

/* convert 64bit int to bytes */
std::vector<unsigned char> bigintToBytes(int64_t paramInt);

uint32_t crc32c(uint32_t crc, const char *buf, size_t len);
