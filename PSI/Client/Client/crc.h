//============================================================================
// Name        : crc.h
// Author      : not me
// Version     : unknow
// Copyright   : not me
// Description : CRC utility for PSI semestral project
//============================================================================

#pragma once
/* CRC-32C (iSCSI) polynomial in reversed bit order. */
#define POLY 0x82f63b78

#include <inttypes.h>

// CRC function
uint32_t crc32c(uint32_t crc, const char *buf, size_t len);