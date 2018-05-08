#pragma once
#include "ReadDataClass.h"

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
				// Make the buffer object.
				ReadDataClass buffer;

				// Set the file size.
				buffer.SetFileSize(dataFile.tellg()); // Get the current position from the end.
				dataFile.seekg(0, std::ios::beg); // Move stream position to beginnig.

				// Test counter.
				int testCnt{};

				// Read the file.
				while (dataFile) {
					// Read the version of network.
					buffer.SetNetVersion(dataFile);

					// Test line.
					std::cout << ++testCnt << " packet, pos = " << dataFile.tellg() << '\n';

					// Read network header.
					switch (buffer.GetNetVersion()) {
						case 0x01: {
							// Do Network V1 stuff.
							// ** TASK 1: Increase number of packets by 1.
							stat.IncreaseDataCnt(NETV1_P);
							// Read Network V1 header.
							buffer.ReadNetV1(dataFile);
							// **TASK 3: Add address to the stat object to be counted later.
							stat.SetAddressNetV1(buffer.m_netV1.sourceAddress);
							stat.SetAddressNetV1(buffer.m_netV1.destAddress);
							// Set transport protocol.
							buffer.SetProtocol();
						} break;
						case 0x02: {
							// Do Network V2 stuff.					
							// ** TASK 2: Increase number of packets by 1.
							stat.IncreaseDataCnt(NETV2_P);
							// Read Network V1 header.
							buffer.ReadNetV2(dataFile);
							// **TASK 3: Add address to the stat object to be counted later.
							stat.SetAddressNetV2(buffer.m_netV2.sourceAddress);
							stat.SetAddressNetV2(buffer.m_netV2.destAddress);
							// Set transport protocol.
							buffer.SetProtocol();
						} break;
						default: {
							// Something goes wrong.
							std::cout << "Couldn't read network version, netVersion = " << buffer.GetNetVersion() << '\n';
							// Stop the reading loop.
							buffer.SetPos(buffer.GetFileSize());
						}
					}

					// Read transport header.
					if (buffer.GetPos() < buffer.GetFileSize()) {
						// Check transport protocol.
						switch (buffer.GetProtocol()) {
							case 0x01: {
								// Go with TRANSPORT V1.
								buffer.ReadTranspV1(dataFile, stat);
							} break;
							case 0x02: {
								// Go with TRANSPORT V2.
								buffer.ReadTranspV2(dataFile,stat);
							} break;
							default: {
								// Something goes wrong.
								std::cout << "Couldn't read transport protocol version, protocol = " << buffer.GetProtocol() << '\n';
								// Stop the reading loop.
								buffer.SetPos(buffer.GetFileSize());
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

	return 0;
}