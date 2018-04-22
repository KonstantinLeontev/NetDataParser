#include <iostream>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>

class NetDataStat {
public:
	NetDataStat() : m_dataSet{}, m_fileName(L"") {}
	NetDataStat(const std::wstring &name) : m_dataSet{}, m_fileName(name){}
	NetDataStat(const NetDataStat &other) {}
	~NetDataStat() {}

	void Set_netV1_Pack(const int &num) { m_dataSet[0] = num; }
	void Set_netV2_Pack(const int &num) { m_dataSet[1] = num; }
	void Set_netV1_Address(const int &num) { m_dataSet[2] = num; }
	void Set_netV2_Address(const int &num) { m_dataSet[3] = num; }
	void Set_transpV1_Pack(const int &num) { m_dataSet[4] = num; }
	void Set_transpV2_Pack(const int &num) { m_dataSet[5] = num; }
	void Set_transpV1_WrongSum(const int &num) { m_dataSet[6] = num; }
	void Set_transpV2_WrongSum(const int &num) { m_dataSet[7] = num; }
	void Set_transpV1_Port(const int &num) { m_dataSet[8] = num; }
	void Set_transpV2_Port(const int &num) { m_dataSet[9] = num; }
	void Set_transpV2_Session(const int &num) { m_dataSet[10] = num; }

	void PrintToScreen();

private:
	// All statistic's numbers stores here.
	int m_dataSet[11];
	std::wstring m_fileName;
};

void NetDataStat::PrintToScreen() {
	std::wcout << L"\nCтатистика по файлу " << m_fileName << '\n' << '\n';
	for (int i = 0; i < 11; i++) {
		std::wcout << i + 1 << ". ";
		switch(i) {
			case 0: std::wcout << L"Количество пакетов NETWORK V1: "; break;
			case 1: std::wcout << L"Количество пакетов NETWORK V2: "; break;
			case 2: std::wcout << L"Количество уникальных адресов устройств NETWORK V1: "; break;
			case 3: std::wcout << L"Количество уникальных адресов устройств NETWORK V2: "; break;
			case 4: std::wcout << L"Количество пакетов TRANSPORT V1: "; break;
			case 5: std::wcout << L"Количество пакетов TRANSPORT V2: "; break;
			case 6: std::wcout << L"Количество пакетов TRANSPORT V1 с ошибочной контрольной суммой: "; break;
			case 7: std::wcout << L"Количество пакетов TRANSPORT V2 с ошибочной контрольной суммой: "; break;
			case 8: std::wcout << L"Количество уникальных портов TRANSPORT V1: "; break;
			case 9: std::wcout << L"Количество уникальных портов TRANSPORT V2: "; break;
			case 10: std::wcout << L"Количество сессий TRANSPORT V2: "; break;
			default: break;
		}
		std::wcout << m_dataSet[i] << '\n';
	}
	std::wcout << '\n';
}

int main() {
	bool run = true;
	std::ifstream dataFile;
	std::wstring fileName;
	_setmode(_fileno(stdout), _O_U16TEXT);


	// Main loop.
	while (run) {
		// Read file here.
		std::wcout << L"Enter a file name to read from ('0' to quit): ";
		std::wcin >> fileName;
		if (fileName != L"0") {
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
				std::wcout << "Couldn't open file " << fileName << "! Try again or quit.\n";
			}
		}
		else {
			run = false;
		}
	}
}