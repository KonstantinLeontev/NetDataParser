#pragma once
#include <iostream>
#include <string>
#include <set>
#include <map>

enum DATA { NETV1_P, NETV2_P, NETV1_A, NETV2_A, TRANSPV1_PACK, TRANSPV2_PACK, TRANSPV1_W, TRANSPV2_W, TRANSPV1_PORT, TRANSPV2_PORT, TRANSPV2_S };

template <typename T>
class NetDataStat {
public:
	NetDataStat() : m_dataSet{}, m_fileName(""), m_addressNet_V1(), m_addressNet_V2(), m_portTransp_V1(), m_portTransp_V2(), fragments() {}
	NetDataStat(const std::string &name) : m_dataSet{}, m_fileName(name), m_addressNet_V1(), m_addressNet_V2(), m_portTransp_V1(), m_portTransp_V2(), fragments() {}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	// Set and get the number of data elements.
	void SetDataCnt(const DATA &data, const T &value) { m_dataSet[data] = value; }
	T GetDataCnt(const DATA &data) const { return m_dataSet[data]; }
	void IncreaseDataCnt(const DATA &data) { m_dataSet[data]++; }

	// Set only unique data elements.
	void SetAddressNetV1(const char* adr);
	void SetAddressNetV2(const char* adr);
	void SetPortTranspV1(const char* p);
	void SetPortTranspV2(const char* p);

	void SetFragments(const unsigned long &num, const char &flag) { fragments.emplace(num, flag); }

	// Convert bytes to unsigned, unsigned long and unsigned long long by choice.
	void BigEndConverter(const int &numOfBytes, const char* buf, unsigned* uNum, unsigned long* ulNum, unsigned long long* ullNum);

	// Get the number of unique elements.
	T GetAddressNetV1Quant() const { return m_addressNet_V1.size(); }
	T GetAddressNetV2Quant() const { return m_addressNet_V2.size(); }
	T GetPortTranspV1() const { return m_portTransp_V1.size(); }
	T GetPortTranspV2() const { return m_portTransp_V2.size(); }

	void PrintToScreen() const;

private:
	// All statistic's numbers stores here.
	T m_dataSet[11];
	std::string m_fileName;

	// Unique adresses and ports.
	std::set<unsigned long> m_addressNet_V1;
	std::set<unsigned long long> m_addressNet_V2;
	std::set<unsigned> m_portTransp_V1;
	std::set<unsigned> m_portTransp_V2;

	// Fragments numbers.
	std::map<unsigned long, char> fragments;
};

template <typename T>
void NetDataStat<T>::SetAddressNetV1(const char* adr) {
	unsigned long address{};
	BigEndConverter(4, adr, 0, &address, 0);
	if (m_addressNet_V1.insert(address).second) {
		IncreaseDataCnt(NETV1_A);
	}
}

template <typename T>
void NetDataStat<T>::SetAddressNetV2(const char* adr) {
	unsigned long long address{};
	BigEndConverter(6, adr, 0, 0, &address);
	if (m_addressNet_V2.insert(address).second) {
		IncreaseDataCnt(NETV2_A);
	}
}

template <typename T>
void NetDataStat<T>::SetPortTranspV1(const char* p) {
	unsigned port{};
	BigEndConverter(2, p, &port, 0, 0);
	if (m_portTransp_V1.insert(port).second) {
		IncreaseDataCnt(TRANSPV1_PORT);
	}
}

template <typename T>
void NetDataStat<T>::SetPortTranspV2(const char* p) {
	unsigned port;
	BigEndConverter(2, p, &port, 0, 0);
	if (m_portTransp_V2.insert(port).second) {
		IncreaseDataCnt(TRANSPV2_PORT);
	}
}

template <typename T>
void NetDataStat<T>::BigEndConverter(const int &numOfBytes, const char* buf, unsigned* uNum, unsigned long* ulNum, unsigned long long* ullNum) {
	switch (numOfBytes) {
	case 2: *uNum = (buf[0] << 8) | buf[1]; break;
	case 4: *ulNum = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3]; break;
	case 6: *ullNum = (buf[0] << 40) | (buf[1] << 32) | (buf[2] << 24) | (buf[3] << 16) | (buf[4] << 8) | buf[5]; break;
	default: std::cout << "\nWrong number of bytes!\n";
	}
}

template <typename T>
void NetDataStat<T>::PrintToScreen() const {
	std::cout << "\nThe statistics of " << m_fileName << '\n' << '\n';
	for (int i = 0; i < 11; i++) {
	std::cout << i + 1 << ". ";
	switch (i) {
	case 0: std::cout << "NETWORK V1 packets: "; break;
	case 1: std::cout << "NETWORK V2 packets: "; break;
	case 2: std::cout << "NETWORK V1 unique addresses: "; break;
	case 3: std::cout << "NETWORK V2 unique addresses: "; break;
	case 4: std::cout << "TRANSPORT V1 packets: "; break;
	case 5: std::cout << "TRANSPORT V2 packets: "; break;
	case 6: std::cout << "TRANSPORT V1 packets with wrong control sum: "; break;
	case 7: std::cout << "TRANSPORT V2 packets with wrong control sum: "; break;
	case 8: std::cout << "TRANSPORT V1 unique ports: "; break;
	case 9: std::cout << "TRANSPORT V2 unique ports: "; break;
	case 10: std::cout << "TRANSPORT V2 sessions: "; break;
	default: break;
	}
	std::cout << m_dataSet[i] << '\n';
	}
	/*std::cout << "\nFragments:\n";
	for (auto& x : fragments) {
		std::cout << "Number: " << x.first << ", flag: " << x.second << '\n';
	}*/
	std::cout << '\n';
}