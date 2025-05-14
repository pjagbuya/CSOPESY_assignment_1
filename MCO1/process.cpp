#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

class Process{
	private:
		string name;
		int core;
		string status;
		int start_time;
		int end_time;

	public:
		void SetName(string name) {
			this->name = name;
		}

		string GetName(){
			return this->name;
		}

		void SetCore(int core) {
			this->core = core;
		}

		int GetCore(){
			return this->core;
		}

		void SetStatus(string status) {
			this->status = status;
		}

		string GetStatus(){
			return this->status;
		}

		void SetStartTime(int start_time) {
			this->start_time = start_time;
		}
	
		int GetStartTime(){
			return this->start_time;
		}

		void SetEndTime(int end_time) {
			this->end_time = end_time;
		}
	
		int GetEndTime(){
			return this->end_time;
		}

		Process(string name, int start_time){
			this->name = name;
			this->core = -1;
			this->status = "Pending";
			this->start_time = start_time;
		}
		
};