// Size of the packet headers.
#define NETV1_HEADER 14

// All structs describes the given net levels.

struct Network_V1 {
	// Version 1 byte.
	char sourceAddress[4];
	char destAddress[4];
	char protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	char dataSize[2];
	char headerChecksum[2];
	// Data 14-N.
};

struct Network_V2 {
	// Version 1 byte.
	char sourceAddress[6];
	char destAddress[6];
	char protocol; // 0x01 - TRANSPORT V1 or 0x02 - TRANSPORT V2
	char dataSize[2];
	char headerChecksum[2];
	// Data 18-N.
};

struct Transport_V1 {
	char sourcePort[2];
	char destPort[2];
	char dataSize[2];
	// Data - 6-N.
	// Checksum - N + 1, N + 2.
};

struct Transport_V2 {
	char sourcePort[2];
	char destPort[2];
	char fragmentNumber[4];
	char lf; // Last 2 bits: F - first packet in the session, L - last packet in the session.
	char dataSize[2];
	// Data - 11-N.
	// Checksum - N + 1, N + 2.
};