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
#define ARR_SIZE 100

#include "console.cpp"

int main() {

	HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE); 
	
	Console console;	

	console.Initialize();
	
	while(!console.GetCommandQuit()){		
        console.Run();
		Sleep(10);
	}
	
    return 0;
}
