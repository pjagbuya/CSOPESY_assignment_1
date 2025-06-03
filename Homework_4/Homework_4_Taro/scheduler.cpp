#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "readyqueue.cpp"
#include "core.cpp"

class Scheduler{
	private:
		vector<ReadyQueue> ready_queues;
		vector<Core> cores;
		string algorithm;
};

