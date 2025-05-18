#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
using namespace std;
#define ARR_SIZE 100

#include "console.cpp"

int main() {

	cout << "Program started" << endl;
	
	Console console;	
	
	int i = 0; 
	COORD console_size = console.GetSize();
	
	while(!console.GetCommandQuit()){		
        console.ConsoleInput();
		console.AssembleScreenOutputList();
		console.ConsoleOut();
		Sleep(50);
		i++;
	}
	
    return 0;
}
