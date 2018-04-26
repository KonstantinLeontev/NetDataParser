#pragma once
#include "NetLevels.h"
#include "NetDataStatClass.h"
#include <fstream>

class ReadDataClass {
public:
	ReadDataClass() : m_netVersion{}, m_protocol{}, m_pos{}, m_fileSize{}, m_dataSize{}, m_checkSum{},
		m_netV1(), m_netV2(), m_transpV1(), m_transpV2() {}

	void ReadNetV1(std::ifstream &dataFile){ dataFile.read((char*)&m_netV1, sizeof(Network_V1)); }
	void ReadNetV2(std::ifstream &dataFile) { dataFile.read((char*)&m_netV2, sizeof(Network_V2)); }
	void ReadTranspV1(std::ifstream &dataFile, NetDataStat<unsigned> &stat);
	void ReadTranspV2(std::ifstream &dataFile, NetDataStat<unsigned> &stat);

	void SetNetVersion(std::ifstream &dataFile) { dataFile.read(&m_netVersion, 1); }
	void SetProtocol();
	void SetPos(const unsigned &pos) { m_pos = pos; }
	void SetFileSize(const unsigned &size) { m_fileSize = size; }

	char GetNetVersion() const { return m_netVersion; }
	char GetProtocol() const { return m_protocol; }
	unsigned GetPos() const { return m_pos; }
	unsigned GetFileSize() const { return m_fileSize; }

	// Buffers for reading headers from file.
	Network_V1 m_netV1;
	Network_V2 m_netV2;
	Transport_V1 m_transpV1;
	Transport_V2 m_transpV2;
	char m_checkSumBuf[2];

private:

	char m_netVersion;
	char m_protocol;
	unsigned m_pos; // Current reading position.
	unsigned m_fileSize;
	unsigned m_dataSize; // A size of the data in the packet.
	unsigned m_checkSum;
};