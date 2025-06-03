#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "process.h"

class Core{
	private:
		string name;
        int id;
        int max_processes;
        int current_processes;
};

