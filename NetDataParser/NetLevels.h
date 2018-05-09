#pragma once
#include <cstdint>

// All structs describes the given net levels.
struct Network_V1 {
	// Version 1 byte.
	uint8_t sourceAddress[4];
	uint8_t destAddress[4];
	uint8_t protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	uint8_t dataSize[2];
	uint8_t headerChecksum[2];
	// Data 14-N.
};

struct Network_V2 {
	// Version 1 byte.
	uint8_t sourceAddress[6];
	uint8_t destAddress[6];
	uint8_t protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	uint8_t dataSize[2];
	uint8_t headerChecksum[2];
	// Data 18-N.
};

struct Transport_V1 {
	uint8_t sourcePort[2];
	uint8_t destPort[2];
	uint8_t dataSize[2];
	// Data - 6-N.
	// Checksum - N + 1, N + 2.
};

struct Transport_V2 {
	uint8_t sourcePort[2];
	uint8_t destPort[2];
	uint8_t fragmentNumber[4];
	uint8_t lf; // Last 2 bits: F - first packet in the session, L - last packet in the session.
	uint8_t dataSize[2];
	// Data - 11-N.
	// Checksum - N + 1, N + 2.
};