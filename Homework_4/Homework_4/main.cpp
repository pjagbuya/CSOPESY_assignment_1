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

#include "scheduler.cpp"

int main() {

	HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE); 

    int time = 0;
	
	Scheduler scheduler = Scheduler();
    scheduler.Initialize();
    while(!scheduler.IsFinished()){
        scheduler.Run(time);
        time++;
    }
    return 0;
}
