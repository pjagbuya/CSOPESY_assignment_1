#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
using namespace std;

#include "console.cpp"

int main() {
	
	Console console;	

	console.Initialize();
	
	while(true){	
		console.Run();
		Sleep(50);
	}
	
    return 0;
}
