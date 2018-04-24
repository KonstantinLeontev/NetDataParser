#include <iostream>
#include <string>
#include <fstream>

class NetDataStat {
public:
	NetDataStat() : m_dataSet{}, m_fileName("") {}
	NetDataStat(const std::string &name) : m_dataSet{}, m_fileName(name){}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	void Set_netV1_Pack(const unsigned &num) { m_dataSet[0] = num; }
	void Set_netV2_Pack(const unsigned &num) { m_dataSet[1] = num; }
	void Set_netV1_Address(const unsigned &num) { m_dataSet[2] = num; }
	void Set_netV2_Address(const unsigned &num) { m_dataSet[3] = num; }
	void Set_transpV1_Pack(const unsigned &num) { m_dataSet[4] = num; }
	void Set_transpV2_Pack(const unsigned &num) { m_dataSet[5] = num; }
	void Set_transpV1_WrongSum(const unsigned &num) { m_dataSet[6] = num; }
	void Set_transpV2_WrongSum(const unsigned &num) { m_dataSet[7] = num; }
	void Set_transpV1_Port(const unsigned &num) { m_dataSet[8] = num; }
	void Set_transpV2_Port(const unsigned &num) { m_dataSet[9] = num; }
	void Set_transpV2_Session(const unsigned &num) { m_dataSet[10] = num; }

	void PrintToScreen();

private:
	// All statistic's numbers stores here.
	unsigned m_dataSet[11];
	std::string m_fileName;
};

void NetDataStat::PrintToScreen() {
	std::cout << "\nThe statistics of " << m_fileName << '\n' << '\n';
	for (int i = 0; i < 11; i++) {
		std::cout << i + 1 << ". ";
		switch(i) {
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
			// Open a file for reading.
			dataFile.open(fileName, std::ios::in);
			// If succeed.
			if (dataFile.is_open()) {
				// Make the data object.
				NetDataStat stat(fileName);
				// Read data statistic.

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