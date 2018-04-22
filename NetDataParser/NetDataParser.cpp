#include <iostream>
#include <string>
#include <fstream>

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