#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "screen.h"

#ifndef PROCESS_H
#define PROCESS_H

class Process{
	private:
		string name;
		int id;
		string status;
		int arrival_time;
		int burst_time;
		int completion_time;
		int turnaround_time;
		int waiting_time;
		int start_time;
		int response_time;
		int priority;
		int current_burst_time;

		Screen screen;

		vector<string> logs;
		string print_str;

	public:
		Process(string name, int id, int arrival_time, int burst_time, int priority, string print_str) : screen(name, 100000){
			this->name = name;
			this->id = id;
			this->status = "New";
			this->arrival_time = arrival_time;
			this->burst_time = burst_time;
			this->completion_time = 0;
			this->turnaround_time = 0;
			this->waiting_time = 0;
			this->start_time = 0;
			this->response_time = 0;
			this->priority = priority;
			this->current_burst_time = burst_time;
			this->print_str = print_str;
		}

		string GetName(){ return name; }
		void SetName(string name){ this->name = name; }
		int GetID(){ return this->id; }
        void SetID( int id ){ this->id = id; }
		string GetStatus(){ return status; }
		void SetStatus(string status){ this->status = status; }
		int GetArrivalTime(){ return arrival_time; }
		void SetArrivalTime(int arrival_time){ this->arrival_time = arrival_time; }
		int GetBurstTime(){ return burst_time; }
		void SetBurstTime(int burst_time){ this->burst_time = burst_time; }
		int GetCompletionTime(){ return completion_time; }
		void SetCompletionTime(int completion_time){ this->completion_time = completion_time; }
		int GetTurnaroundTime(){ return turnaround_time; }
		void SetTurnaroundTime(int turnaround_time){ this->turnaround_time = turnaround_time; }
		int GetWaitingTime(){ return waiting_time; }
		void SetWaitingTime(int waiting_time){ this->waiting_time = waiting_time; }
		int GetStartTime(){ return start_time; }
		void SetStartTime(int start_time){ this->start_time = start_time; }
		int GetResponseTime(){ return response_time; }
		void SetResponseTime(int response_time){ this->response_time = response_time; }
		int GetPriority(){ return priority; }
		void SetPriority(int priority){ this->priority = priority; }
		int GetCurrentBurstTime(){ return current_burst_time; }
		void SetCurrentBurstTime(int current_burst_time){ this->current_burst_time = current_burst_time; }
		vector<string> GetLogs(){ return logs; }
		void SetLogs(vector<string> logs){ this->logs = logs; }
		string GetPrintStr(){ return print_str; }
		void SetPrintStr(string print_str){ this->print_str = print_str; }

		string GetTime(){
			time_t timestamp;
			time(&timestamp);
			char* time_str = ctime(&timestamp);
			string time_string(time_str);
			time_string.erase(time_string.length() - 1);
			return time_string;
		}

		void ReadyProcess(){
			this->status = "Ready";
		}

		void StartProcess(int current_time){
			if(this->status == "Blocked"){
				this->status = "Running";
			}
			else{
				this->status = "Running";
				this->start_time = current_time;
			}
		}

		void RunProcess(int core_id){
			this->current_burst_time--;
			this->logs.push_back("(" + this->GetTime() + ") Core:" + to_string(core_id) + "\"" + print_str + "\"");
			this->screen.Run(this->print_str);
		}

		void HaltProcess(){
			this->status = "Blocked";
		}

		void FinishProcess(int current_time){
			this->status = "Finished";
			this->completion_time = current_time;
			this->turnaround_time = this->completion_time - this->arrival_time;
			this->waiting_time = this->turnaround_time - this->burst_time;
			this->response_time = this->start_time - this->arrival_time;
			this->WriteLogs();
		}

		bool IsProcessFinished() {
			return this->status == "Finished";
		}

		void WriteLogs(){
			ofstream file("output/" + this->name + ".txt");

			file << "--------------------------------------------------------" << "\n";

			file << "Process: " << this->name << "\n";

			file << "--------------------------------------------------------" << "\n";

			for(auto log : logs){
				file << log << "\n";
			}

			file << "--------------------------------------------------------" << "\n";

			file << "Arrival Time: " << this->arrival_time << "\n";
			file << "Burst Time: " << this->burst_time << "\n";
			file << "Completion Time: " << this->completion_time << "\n";
			file << "Turnaround Time: " << this->turnaround_time << "\n";
			file << "Waiting Time: " << this->waiting_time << "\n";
			file << "Start Time: " << this->start_time << "\n";
			file << "Response Time: " << this->response_time << "\n";
			file << "Priority: " << this->priority << "\n";

			file.close();
		}

};

#endif // PROCESS_H