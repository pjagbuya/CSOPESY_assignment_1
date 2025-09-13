#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "file.cpp"

class Process{
	private:
		string name;
		int core;
		int pc;
		string status;
		int start_time;
		int end_time;
		File file;

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

		void SetFile(const File& file) {
			this->file = file;
		}

		File GetFile() {
			return this->file;
		}

		Process(string name, int start_time){
			this->name = name;
			this->core = -1;
			this->status = "Pending";
			this->start_time = start_time;
		}
		
		void RandomizeProcess(int count) {
			for (int i = 0; i < count; ++i) {
				string hex_value = "0x" + to_string(rand() % 256); // Random hex value
				Hex hex(hex_value);
				this->file.AddInstruction(hex);
			}
		}
};
