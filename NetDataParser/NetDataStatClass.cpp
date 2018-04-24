#pragma once
#include "NetDataStatClass.h"

template <typename T>
void NetDataStat::PrintToScreen() {
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
	std::cout << '\n';
}

template <typename T>
void NetDataStat<T>::SetData(const DATA &data, const T &value) {
	m_dataSet[data] = value;
}

template <typename T>
T NetDataStat<T>::GetData(const DATA &data) {
	return m_dataSet[data];
}

template <typename T>
void NetDataStat<T>::IncreaseData(const DATA &data, const T &value) {
	m_dataSet[data] += value;
}