#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
using namespace std;

#include "screen.h"

class Header : public Screen {
	public:
		Header(int max_height) : Screen("header", max_height){
			
		}
		void AssembleOutputList() override {
			vector<string> header = {
			"  _____  _____  ____  _____  ______  _______     __",
			" / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / /",
			"| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ / ",
			"| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /  ",
			"| |____ ____) | |__| | |    | |____ ____) |  | |   ",
			" \\_____|_____/ \\____/|_|    |______|_____/   |_|   ",
			" Hello, Welcome to CSOPESY CommandLine!            ",
			" Type 'exit' to quit, 'clear' to clear the screen  "
			};
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}

			this->SetOutputSize(header.size());
		}
};
