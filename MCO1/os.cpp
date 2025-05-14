#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "scheduler.cpp"
#include "cpu.cpp"
#include "memory.cpp"
#include "storage.cpp"

class Os{
	private:
		bool is_initialize;
		Cpu cpu;
        Memory memory;
        Storage storage;
		Scheduler scheduler;
		
	public:
		Os(){
			this->is_initialize = false;
		}
		
		void Init(){
			this->is_initialize = true;
			this->Config("config.txt");
		}
		
		void Config(string file_name){
			// read file for config details
			/*
			this->num_cpu;
			this->scheduler.SetMode("");
			this->quantum_cycles;
			this->batch_process_freq;
			this->min_ins;
			this->max_ins;
			this->delays_per_exec;
			*/
		}
		
		bool IsInit(){
			return this->is_initialize;			
		}
	
};