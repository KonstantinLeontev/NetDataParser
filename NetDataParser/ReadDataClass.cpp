#include "ReadDataClass.h"

void ReadDataClass::CheckWrongSum(std::ifstream &dataFile, NetDataStat<unsigned> &stat, const unsigned short &headerSize, const DATA value) {
	// Jump to the checksum field and read the checksum.
	m_pos = dataFile.tellg();
	m_pos += m_dataSize;
	dataFile.seekg(m_pos);
	// Read the checksum
	dataFile.read((char*)&m_checkSumBuf, sizeof(char[2]));

	// Set checksum member of the class.
	stat.BigEndConverter(2, m_checkSumBuf, &m_checkSum, 0, 0);

	// Check the size of the whole packet and checksum.
	uint16_t tempSum = (headerSize + m_dataSize) ^ 0xFFFF;
	if (m_checkSum != tempSum) {
		// **TASK 7: CheckSum is wrong - increase corresponding field in the stat array.
		stat.IncreaseDataCnt(value);
	}
}

void ReadDataClass::ReadTranspV1(std::ifstream &dataFile, NetDataStat<unsigned> &stat) {
	dataFile.read((char*)&m_transpV1, sizeof(Transport_V1));
	// **TASK 5: Increase the counter for TRANSPORT V1 packets.
	stat.IncreaseDataCnt(TRANSPV1_PACK);
	// **TASK 9: Add the ports to the set.
	stat.SetPortTranspV1(m_transpV1.sourcePort);
	stat.SetPortTranspV1(m_transpV1.destPort);

	// Get the size of the data in the packet.
	stat.BigEndConverter(2, m_transpV1.dataSize, &m_dataSize, 0, 0);

	// Check for wrong sum.
	CheckWrongSum(dataFile, stat, TRANSPV1_HEADER, TRANSPV1_W);
}

void ReadDataClass::ReadTranspV2(std::ifstream &dataFile, NetDataStat<unsigned> &stat) {
	dataFile.read((char*)&m_transpV2, sizeof(Transport_V2));
	// **TASK 6: Increase the counter for TRANSPORT V2 packets.
	stat.IncreaseDataCnt(TRANSPV2_PACK);
	// **TASK 10: Add the ports to the set.
	stat.SetPortTranspV2(m_transpV2.sourcePort);
	stat.SetPortTranspV2(m_transpV2.destPort);

	// Get the size of the data in the packet.
	stat.BigEndConverter(2, m_transpV2.dataSize, &m_dataSize, 0, 0);

	// Check for wrong sum.
	CheckWrongSum(dataFile, stat, TRANSPV2_HEADER, TRANSPV2_W);

	// Set the fragment number.
	stat.BigEndConverter(4, m_transpV2.fragmentNumber, 0, &m_fragNum, 0);
	// Checking for packet flag.
	uint8_t bit1 = (m_transpV2.lf >> 1) & 1U;
	uint8_t bit0 = (m_transpV2.lf >> 0) & 1U;
	// Add this numbers to the stat databse.
	if (bit1) {
		stat.SetFragments(m_fragNum, 'L');
	}
	else if (bit0) {
		stat.SetFragments(m_fragNum, 'F');
	}
	else {
		stat.SetFragments(m_fragNum, '-');
	}
}

void ReadDataClass::SetProtocol() {
	if (m_netVersion == 0x01) {
		m_protocol = m_netV1.protocol;
	}
	else {
		m_protocol = m_netV2.protocol;
	}
}