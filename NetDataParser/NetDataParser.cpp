#pragma once
#include <fstream>
#include "NetDataStatClass.h"
#include "NetLevels.h"

void readTranspV1(
	std::ifstream &dataFile,
	Transport_V1 &transpV1,
	NetDataStat<unsigned> &stat,
	unsigned &pos
) {
	dataFile.read((char*)&transpV1, sizeof(Transport_V1));
	// **TASK 5: Increase the counter for TRANSPORT V1 packets.
	stat.IncreaseDataCnt(TRANSPV1_PACK);
	// **TASK 9: Add the ports to the set.
	stat.SetPortTranspV1(transpV1.sourcePort);
	stat.SetPortTranspV2(transpV1.destPort);

	// Jump pos to the end of the packet to read next one.
	unsigned dataSize = (transpV1.dataSize[0] << 8) | transpV1.dataSize[1];
	// Get the new pos at the next packet beginning.
	pos = dataFile.tellg();
	pos += dataSize + 2;
	// Set stream pointer to the new pos.
	dataFile.seekg(pos);
}

void readTranspV2(
	std::ifstream &dataFile,
	Transport_V2 &transpV2,
	NetDataStat<unsigned> &stat,
	unsigned &pos
) {
	dataFile.read((char*)&transpV2, sizeof(Transport_V2));
	// **TASK 6: Increase the counter for TRANSPORT V2 packets.
	stat.IncreaseDataCnt(TRANSPV2_PACK);
	// **TASK 10: Add the ports to the set.
	stat.SetPortTranspV2(transpV2.sourcePort);
	stat.SetPortTranspV2(transpV2.destPort);

	// Jump pos to the end of the packet to read next one.
	unsigned dataSize = (transpV2.dataSize[0] << 8) | transpV2.dataSize[1];
	// Get the new pos at the next packet beginning.
	pos = dataFile.tellg();
	pos += dataSize + 2;
	// Set stream pointer to the new pos.
	dataFile.seekg(pos);
}

int main() {
	bool run = true;
	std::ifstream dataFile;
	std::string fileName;

	// Main loop.
	while (run) {
		// Read file here.
		std::cout << "Enter a file name to read from ('0' to quit): ";
		std::cin >> fileName;
		if (fileName != "0") {
			// Open a file for reading from the end.
			dataFile.open(fileName, std::ios::in | std::ios::binary | std::ios::ate);
			// If succeed.
			if (dataFile.is_open()) {
				// Make the data object.
				NetDataStat<unsigned> stat(fileName);

				// Structs for reading to.
				Network_V1 netV1 = {};
				Network_V2 netV2 = {};
				Transport_V1 transpV1 = {};
				Transport_V2 transpV2 = {};
				char netVersion{}, protocol{};

				// Check the file size.
				unsigned pos{}, size{};
				size = dataFile.tellg(); // Get the current position from the end.
				dataFile.seekg(0, std::ios::beg); // Move stream position to beginnig.

				// Read the file.
				while (pos < size) {
					// Read the version of network.
					dataFile.read(&netVersion, 1);

					// Read network header.
					switch (netVersion) {
						case 0x01: {
							// Do Network V1 staff.
							// ** TASK 1: Increase number of packets by 1.
							stat.IncreaseDataCnt(NETV1_P);
							// Read Network V1 header.
							dataFile.read((char*)&netV1, sizeof(Network_V1));
							// **TASK 3: Add address to the stat object to be counted later.
							stat.SetAddressNetV1(netV1.sourceAddress);
							stat.SetAddressNetV1(netV1.destAddress);
							// Set current transport protocol.
							protocol = netV1.protocol;
						} break;
						case 0x02: {
							// Do Network V2 staff.					
							// ** TASK 2: Increase number of packets by 1.
							stat.IncreaseDataCnt(NETV2_P);
							// Read Network V1 header.
							dataFile.read((char*)&netV2, sizeof(Network_V2));
							// **TASK 3: Add address to the stat object to be counted later.
							stat.SetAddressNetV2(netV2.sourceAddress);
							stat.SetAddressNetV2(netV2.destAddress);
							// Set current transport protocol.
							protocol = netV2.protocol;
						} break;
						default: {
							// Something goes wrong.
							std::cout << "Couldn't read network version, netVersion = " << netVersion << '\n';
							// Stop the reading loop.
							pos = size;
						}
					}

					// Read transport header.
					if (pos != size) {
						// Check transport protocol.
						switch (protocol) {
							case 0x01: {
								// Go with TRANSPORT V1.
								readTranspV1(dataFile, transpV1, stat, pos);
							} break;
							case 0x02: {
								// Go with TRANSPORT V2.
								readTranspV2(dataFile, transpV2, stat, pos);
							} break;
							default: {
								// Something goes wrong.
								std::cout << "Couldn't read transport protocol version, protocol = " << protocol << '\n';
								// Stop the reading loop.
								pos = size;
							}
						}
					}
				}

				// Print statistics to the screen.
				stat.PrintToScreen();
				
				// Close file.
				dataFile.close();
			}
			else {
				std::cout << "Couldn't open file " << fileName << "! Try again or quit.\n";
			}
		}
		else {
			run = false;
		}
	}
}