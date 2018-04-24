#pragma once
#include <iostream>
#include <string>
#include <fstream>

enum DATA { NETV1_P, NETV2_P, NETV1_A, NETV1_A, TRANSPV1_PACK, TRANSPV2_PACK, TRANSPV1_W, TRANSPV2_W, TRANSPV1_PORT, TRANSPV2_PORT, TRANSPV2_S };

template <typename T>
class NetDataStat {
public:
	NetDataStat() : m_dataSet{}, m_fileName("") {}
	NetDataStat(const std::string &name) : m_dataSet{}, m_fileName(name){}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	void SetData(const DATA &data, const T &value);
	T GetData(const DATA &data);
	void IncreaseData(const DATA &data, const T &value);

	void PrintToScreen();

private:
	// All statistic's numbers stores here.
	T m_dataSet[11];
	std::string m_fileName;
};