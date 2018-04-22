#include <iostream>
#include <string>
#include <fstream>

class NetDataStat {
public:
	NetDataStat() :
		m_netV1_Pack(0), m_netV2_Pack(0), m_netV1_Address(0), m_netV2_Address(0), m_transpV1_Pack(0),
		m_transpV2_Pack(0), m_transpV1_WrongSum(0), m_transpV2_WrongSum(0), m_transpV1_Port(0),
		m_transpV2_Port(0), m_transpV2_Session(0) {}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	void Set_netV1_Pack(const int &num) { m_netV1_Pack = num; }
	void Set_netV2_Pack(const int &num) { m_netV2_Pack = num; }
	void Set_netV1_Address(const int &num) { m_netV1_Address = num; }
	void Set_netV2_Address(const int &num) { m_netV2_Address = num; }
	void Set_transpV1_Pack(const int &num) { m_transpV1_Pack = num; }
	void Set_transpV2_Pack(const int &num) { m_transpV2_Pack = num; }
	void Set_transpV1_WrongSum(const int &num) { m_transpV1_WrongSum = num; }
	void Set_transpV2_WrongSum

private:
	int m_netV1_Pack;
	int m_netV2_Pack;
	int m_netV1_Address;
	int m_netV2_Address;
	int m_transpV1_Pack;
	int m_transpV2_Pack;
	int m_transpV1_WrongSum;
	int m_transpV2_WrongSum;
	int m_transpV1_Port;
	int m_transpV2_Port;
	int m_transpV2_Session;	
};

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
				// Read data statistic.
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