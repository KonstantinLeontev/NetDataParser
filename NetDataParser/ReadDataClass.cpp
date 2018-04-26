#include "ReadDataClass.h"

void ReadDataClass::ReadTranspV1(std::ifstream &dataFile, NetDataStat<unsigned> &stat) {
	dataFile.read((char*)&m_transpV1, sizeof(Transport_V1));
	// **TASK 5: Increase the counter for TRANSPORT V1 packets.
	stat.IncreaseDataCnt(TRANSPV1_PACK);
	// **TASK 9: Add the ports to the set.
	stat.SetPortTranspV1(m_transpV1.sourcePort);
	stat.SetPortTranspV2(m_transpV1.destPort);

	// Get the size of the data in the packet.
	m_dataSize = (m_transpV1.dataSize[0] << 8) | m_transpV1.dataSize[1];

	// Jump to the checksum field and read the checksum.
	m_pos = dataFile.tellg();
	m_pos += m_dataSize;
	// Read the checksum
	//unsigned checkSum = dataFile.read();

	// Check the size of the whole packet and checksum.
	//unsigned headerChecksum = (transpV1.headerChecksum[0] << 8 | )


	// Get the new pos at the next packet beginning.
	m_pos = dataFile.tellg();
	m_pos += m_dataSize + 2;
	// Set stream pointer to the new pos.
	dataFile.seekg(m_pos);
}

void ReadDataClass::ReadTranspV2(std::ifstream &dataFile, NetDataStat<unsigned> &stat) {
	dataFile.read((char*)&m_transpV2, sizeof(Transport_V2));
	// **TASK 6: Increase the counter for TRANSPORT V2 packets.
	stat.IncreaseDataCnt(TRANSPV2_PACK);
	// **TASK 10: Add the ports to the set.
	stat.SetPortTranspV2(m_transpV2.sourcePort);
	stat.SetPortTranspV2(m_transpV2.destPort);

	// Jump pos to the end of the packet to read next one.
	m_dataSize = (m_transpV2.dataSize[0] << 8) | m_transpV2.dataSize[1];
	// Get the new pos at the next packet beginning.
	m_pos = dataFile.tellg();
	m_pos += m_dataSize + 2;
	// Set stream pointer to the new pos.
	dataFile.seekg(m_pos);
}

void ReadDataClass::SetProtocol() {
	if (m_netVersion == 0x01) {
		m_protocol = m_netV1.protocol;
	}
	else {
		m_protocol = m_netV2.protocol;
	}
}