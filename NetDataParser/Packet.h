#pragma once

struct Fragment {
	uint32_t fragment;
	unsigned char flag;

	bool operator < (const Fragment &other) const { return fragment < other.fragment; }
	bool operator == (const Fragment &other) const {
		return
			(fragment == other.fragment) &&
			(flag == other.flag);
	}
};

struct Packet {
	uint64_t sourceAddress;
	uint64_t destAddress;
	uint16_t sourcePort;
	uint16_t destPort;

	bool operator < (const Packet &other) const {
		return sourceAddress < other.sourceAddress;
	}

	bool operator == (const Packet &other) const {
		return
			(sourceAddress == other.sourceAddress) &&
			(destAddress == other.destAddress) &&
			(sourcePort == other.sourcePort) &&
			(destPort == other.destPort);
	}
}; 