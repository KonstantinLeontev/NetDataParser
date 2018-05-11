#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include "Packet.h"

enum DATA {
	NETV1_P, // Task 1: NETWORK V1 packets.
	NETV2_P, // Task 2: NETWORK V2 packets.
	NETV1_A, // Task 3: NETWORK V1 addresses.
	NETV2_A, // Task 4: NETWORK V2 addresses.
	TRANSPV1_PACK, // Task 5: TRANSPORT V1 packets.
	TRANSPV2_PACK, // Task 6: TRANSPORT V2 packets.
	TRANSPV1_W, // Task 7: TRANSPORT V1 packets with wrong checksum.
	TRANSPV2_W, // Task 8: TRANSPORT V2 packets with wrong checksum.
	TRANSPV1_PORT, // Task 9: TRANSPORT V1 ports.
	TRANSPV2_PORT, // Task 10: TRANSPORT V2 ports.
	TRANSPV2_S // Task 11: TRANSPORT V2 sessions.
};

template <typename T>
class NetDataStat {
public:
	NetDataStat() : m_dataSet{}, m_fileName(""), m_addressNet_V1(), m_addressNet_V2(), m_portTransp_V1(), m_portTransp_V2(), m_packet(), m_fragment(),
		m_V1_address{}, m_V2_address{}, m_V2_port{} {}
	NetDataStat(const std::string &name) :
		m_dataSet{}, m_fileName(name), m_addressNet_V1(), m_addressNet_V2(), m_portTransp_V1(), m_portTransp_V2(), m_packet(), m_fragment(),
		m_V1_address{}, m_V2_address{}, m_V1_port{}, m_V2_port{} {}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	// Increase the counter of data elements.
	void IncreaseDataCnt(const DATA &data) { m_dataSet[data]++; }

	// Set only unique data elements.
	void SetAddressNetV1(const uint8_t* addr_1, const uint8_t* addr_2);
	void SetAddressNetV2(const uint8_t* addr_1, const uint8_t* addr_2);
	void SetPortTranspV1(const uint8_t* p1, const uint8_t* p2);
	void SetPortTranspV2(const uint8_t* p1, const uint8_t* p2);

	// Adds packet to the session map.
	void SetSession(const Transport_V2 &transp_V2, const unsigned short &netVersion);
	// Set the general session's counter. 
	void SetSessionCnt();

	// Convert bytes to unsigned, uint32_t and uint64_t by choice.
	void BigEndConverter(const int &numOfBytes, const uint8_t* buf, uint16_t* uNum, uint32_t* ulNum, uint64_t* ullNum);

	void PrintToScreen() const;
	void PrintSessions();

private:
	// All statistic's numbers stores here.
	T m_dataSet[11];
	std::string m_fileName;

	// Unique addresses and ports.
	std::set<uint32_t> m_addressNet_V1;
	std::set<uint64_t> m_addressNet_V2;
	std::set<uint16_t> m_portTransp_V1;
	std::set<uint16_t> m_portTransp_V2;

	// Addresses and port for session calculation.
	uint32_t m_V1_address[2]; // [0] - source, [1] - destination.
	uint64_t m_V2_address[2];
	uint16_t m_V1_port[2];
	uint16_t m_V2_port[2];

	// Sessions.
	std::map<Packet, std::set<Fragment> > m_sessions;
	Packet m_packet;
	Fragment m_fragment;
};

template<typename T>
void NetDataStat<T>::SetSession(const Transport_V2 &transp_V2, const unsigned short &netVersion) {
	// Fill packet struct from stat object.
	switch (netVersion) {
		case 1: {
			m_packet.sourceAddress = m_V1_address[0];
			m_packet.destAddress = m_V1_address[1];
			break;
		}
		case 2: {
			m_packet.sourceAddress = m_V2_address[0];
			m_packet.destAddress = m_V2_address[1];
			break;
		}
		default: std::cout << "\nWrong net version value!\n\n";
	}
	m_packet.sourcePort = m_V2_port[0];
	m_packet.destPort = m_V2_port[1];

	// Read fragment and flag from raw bytes.
	BigEndConverter(4, transp_V2.fragmentNumber, 0, &m_fragment.fragment, 0);
	if (transp_V2.lf & 2) {
		m_fragment.flag = 'L';
	}
	else if (transp_V2.lf & 1) {
		m_fragment.flag = 'F';
	}
	else {
		m_fragment.flag = '-';
	}

	// Add packet's data to the session map.
	m_sessions[m_packet].insert(m_fragment);
}

template<typename T>
void NetDataStat<T>::SetSessionCnt() {
	T temp = m_sessions.size();
	for (auto it : m_sessions) {
		if ((it.second.begin())->flag == 'F' && (--it.second.end())->flag == 'L') {
			if (adjacent_find(it.second.begin(), it.second.end()) != it.second.end()) {
				temp--;
			}
		}
		else {
			temp--;
		}
	}
	m_dataSet[TRANSPV2_S] = temp;
}

template <typename T>
void NetDataStat<T>::SetAddressNetV1(const uint8_t* addr_1, const uint8_t* addr_2) {
	BigEndConverter(4, addr_1, 0, &m_V1_address[0], 0); // Source address.
	BigEndConverter(4, addr_2, 0, &m_V1_address[1], 0); // Destination address.

	// Check for unique address and increase the stat counter if it is so.
	for (int i = 0; i < 2; i++) {
		if (m_addressNet_V1.insert(m_V1_address[i]).second) {
			IncreaseDataCnt(NETV1_A);
		}
	}
}

template <typename T>
void NetDataStat<T>::SetAddressNetV2(const uint8_t* addr_1, const uint8_t* addr_2) {
	BigEndConverter(6, addr_1, 0, 0, &m_V2_address[0]); // Source address.
	BigEndConverter(6, addr_2, 0, 0, &m_V2_address[1]); // Destination address.

	// Check for unique address and increase the stat counter if it is so.
	for (int i = 0; i < 2; i++) {
		if (m_addressNet_V2.insert(m_V2_address[i]).second) {
			IncreaseDataCnt(NETV2_A);
		}
	}
}

template <typename T>
void NetDataStat<T>::SetPortTranspV1(const uint8_t* p1, const uint8_t* p2) {
	BigEndConverter(2, p1, &m_V1_port[0], 0, 0); // Source address.
	BigEndConverter(2, p2, &m_V1_port[1], 0, 0); // Destination address.

	// Check for unique address and increase the stat counter if it is so.
	for (int i = 0; i < 2; i++) {
		if (m_portTransp_V1.insert(m_V1_port[i]).second) {
			IncreaseDataCnt(TRANSPV1_PORT);
		}
	}
}

template <typename T>
void NetDataStat<T>::SetPortTranspV2(const uint8_t* p1, const uint8_t* p2) {
	BigEndConverter(2, p1, &m_V2_port[0], 0, 0); // Source address.
	BigEndConverter(2, p2, &m_V2_port[1], 0, 0); // Destination address.

	// Check for unique address and increase the stat counter if it is so.
	for (int i = 0; i < 2; i++) {
		if (m_portTransp_V2.insert(m_V2_port[i]).second) {
			IncreaseDataCnt(TRANSPV2_PORT);
		}
	}
}

template <typename T>
void NetDataStat<T>::BigEndConverter(const int &numOfBytes, const uint8_t* buf, uint16_t* uNum, uint32_t* ulNum, uint64_t * ullNum) {
	switch (numOfBytes) {
	case 2: {
		*uNum = buf[0];
		*uNum = (*uNum << 8) | buf[1];
		break;
	}
	case 4: *ulNum = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]; break;
	case 6: *ullNum = (buf[0] << 40) | (buf[1] << 32) | (buf[2] << 24) | (buf[3] << 16) | (buf[4] << 8) | buf[5]; break;
	default: std::cout << "\nWrong number of bytes!\n";
	}
}

template <typename T>
void NetDataStat<T>::PrintToScreen() const {
	std::cout << "\nThe statistics of " << m_fileName << '\n' << '\n';
	for (int i = 0; i < 11; i++) {
		std::cout << std::right << std::setfill('0') << std::setw(2) << i + 1 << ". ";
		switch (i) {
			case 0: std::cout << std::setfill(' ') << std::setw(50) << std::left << "NETWORK V1 packets: "; break;
			case 1: std::cout << std::setfill(' ') << std::setw(50) << std::left << "NETWORK V2 packets: "; break;
			case 2: std::cout << std::setfill(' ') << std::setw(50) << std::left << "NETWORK V1 unique addresses: "; break;
			case 3: std::cout << std::setfill(' ') << std::setw(50) << std::left << "NETWORK V2 unique addresses: "; break;
			case 4: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V1 packets: "; break;
			case 5: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V2 packets: "; break;
			case 6: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V1 packets with wrong control sum: "; break;
			case 7: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V2 packets with wrong control sum: "; break;
			case 8: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V1 unique ports: "; break;
			case 9: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V2 unique ports: "; break;
			case 10: std::cout << std::setfill(' ') << std::setw(50) << std::left << "TRANSPORT V2 sessions: "; break;
			default: break;
		}
		std::cout << std::setw(16) << m_dataSet[i] << '\n';
	}
	std::cout << '\n';

	/*std::cout << "\nFragments:\n";
	for (auto& x : fragments) {
		std::cout << "Number: " << x.first << ", flag: " << x.second << '\n';
	}
	std::cout << '\n';*/
}

// Testing method for sessions checking.
template<typename T>
void NetDataStat<T>::PrintSessions() {
	unsigned i{1};

	std::cout << "\n--- Sessions: ---\n\n";
	// Iterate trough the sessions map.
	for (std::map<Packet, std::set<Fragment> >::iterator itM = m_sessions.begin(); itM != m_sessions.end(); ++itM) {
		std::cout << "Session " << i << ":\n" <<
			"	source address " << itM->first.sourceAddress << '\n' <<
			"	destination address " << itM->first.destAddress << '\n' <<
			"	source port " << itM->first.sourcePort << '\n' <<
			"	destination port " << itM->first.destPort << '\n' <<
			"	Fragments:\n";
		for (std::set<Fragment>::iterator itS = itM->second.begin(); itS != itM->second.end(); ++itS) {
			std::cout << "		fragment: " << itS->fragment << ", flag: " << itS->flag << '\n';
		}
		i++;
	}
}