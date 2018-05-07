// Header sizes with checksum. 
#define TRANSPV1_HEADER 8
#define TRANSPV2_HEADER 13

// All structs describes the given net levels.
struct Network_V1 {
	// Version 1 byte.
	unsigned char sourceAddress[4];
	unsigned char destAddress[4];
	unsigned char protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	unsigned char dataSize[2];
	unsigned char headerChecksum[2];
	// Data 14-N.
};

struct Network_V2 {
	// Version 1 byte.
	unsigned char sourceAddress[6];
	unsigned char destAddress[6];
	unsigned char protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	unsigned char dataSize[2];
	unsigned char headerChecksum[2];
	// Data 18-N.
};

struct Transport_V1 {
	unsigned char sourcePort[2];
	unsigned char destPort[2];
	unsigned char dataSize[2];
	// Data - 6-N.
	// Checksum - N + 1, N + 2.
};

struct Transport_V2 {
	unsigned char sourcePort[2];
	unsigned char destPort[2];
	unsigned char fragmentNumber[4];
	unsigned char lf; // Last 2 bits: F - first packet in the session, L - last packet in the session.
	unsigned char dataSize[2];
	// Data - 11-N.
	// Checksum - N + 1, N + 2.
};