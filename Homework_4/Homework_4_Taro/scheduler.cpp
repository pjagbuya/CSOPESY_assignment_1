#include <iostream>
#include <fstream>
#include <string>
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
		vector<Process> initial_queue;
		vector<ReadyQueue> ready_queues;
		vector<Process> finish_queue;
		vector<Core> cores;
		string algorithm;
		string type;
		int core_count;
		int queue_count;
		int max_priority;
	
	public:
		Scheduler(){
			
		}

		void SetInitialQueue(const std::vector<Process>& initial_queue) { this->initial_queue = initial_queue; }
		std::vector<Process> GetInitialQueue() { return this->initial_queue; }
		void SetReadyQueues(const std::vector<ReadyQueue>& ready_queues) { this->ready_queues = ready_queues; }
		std::vector<ReadyQueue> GetReadyQueues() { return this->ready_queues; }
		void SetFinishQueue(const std::vector<Process>& finish_queue) { this->finish_queue = finish_queue; }
		std::vector<Process> GetFinishQueue() { return this->finish_queue; }
		void SetCores(const std::vector<Core>& cores) { this->cores = cores; }
		std::vector<Core> GetCores() { return this->cores; }
		void SetAlgorithm(const std::string& algorithm) { this->algorithm = algorithm; }
		std::string GetAlgorithm() { return this->algorithm; }
		void SetType(const std::string& type) { this->type = type; }
		std::string GetType() { return this->type; }
		void SetCoreCount(int core_count) { this->core_count = core_count; }
		int GetCoreCount() { return this->core_count; }
		void SetQueueCount(int queue_count) { this->queue_count = queue_count; }
		int GetQueueCount() { return this->queue_count; }
		void SetMaxPriority(int max_priority) { this->max_priority = max_priority; }
		int GetMaxPriority() { return this->max_priority; }

		void Initialize(){

			smatch match;
			vector<string> queue_algo;
			vector<int> queue_tq;

			ifstream file("settings.txt");
			if (!file.is_open()) {
				cerr << "Failed to open the file.\n";
			}
			else{
				string line;
				while (getline(file, line)) {
					if(regex_match(line, match, (regex)(R"(Algo: (\s+))"))){
						this->algorithm = match[2].str();
					}
					else if(regex_match(line, match, (regex)(R"(Type: (\s+))"))){
						this->type = match[2].str();
					}
					else if(regex_match(line, match, (regex)(R"(Core: (\d+))"))){
						this->core_count = stoi(match[2].str());
					}
					else if(regex_match(line, match, (regex)(R"(Queue: (\d+))"))){
						this->queue_count = stoi(match[2].str());
					}
					else if(regex_match(line, match, (regex)(R"(MaxP: (\d+))"))){
						this->max_priority = stoi(match[2].str());
					}
					else if(regex_match(line, match, (regex)(R"(Queue (\d+): (\s+) (\d+))"))){
						queue_algo.push_back(match[3].str());
						queue_tq.push_back(stoi(match[4].str()));
					}
				}
				file.close();
			}

			for(int i = 0; i < this->core_count; i++){
				Core core = Core(i);
				this->cores.push_back(core);
			}

			for(int i = 0; i < this->queue_count; i++){
				ReadyQueue ready_queue = ReadyQueue(i, queue_algo[i], queue_tq[i], i);
				this->ready_queues.push_back(ready_queue);
			}
			
		}

		void Import() {
			this->initial_queue = {
				Process("P1", 0, 5, 2, "A"),
				Process("P2", 1, 3, 1, "B"),
				Process("P3", 2, 8, 3, "C"),
				Process("P4", 3, 6, 2, "D"),
				Process("P5", 4, 2, 4, "E"),
				Process("P6", 5, 4, 1, "F"),
				Process("P7", 6, 7, 2, "G"),
				Process("P8", 7, 1, 5, "H"),
				Process("P9", 8, 9, 3, "I"),
				Process("P10", 9, 2, 2, "J")
			};
		}

		void Insert(int current_time){
			for(auto process : initial_queue){
				if(current_time == process.GetArrivalTime()){
					if(this->algorithm != "MLQ" || this->algorithm != "MLFQ"){
						this->Arrive(process, 0);
					}
					else{
						this->Arrive(process, (int)(process.GetPriority() / (this->max_priority / this->core_count)));
					}
				}
			}
		}

		void Arrive(Process process, int index){
			this->ready_queues[index].AddProcess(process);
		}

		bool IsFinished(){
			return false;
		}

		void ContextSwitch(int core_id, int ready_queue_id, int process_id){
			
		}

		void FirstComeFirstServe(int cores, int core_id, int ready_queue_id, int current_time){
			
		}

		void CommonCore(int current_time){
			if(this->algorithm == "FCFS"){
				this->FirstComeFirstServe(this->core_count, 0, 0, current_time);
			}
			if(this->algorithm == "SJS"){

			}
			if(this->algorithm == "SRTS"){

			}
			if(this->algorithm == "RR"){

			}
			if(this->algorithm == "PS"){

			}
			if(this->algorithm == "MLQ"){

			}
			if(this->algorithm == "MLFQ"){

			}
		}

		void SingleCore(int current_time){
			if(this->algorithm == "FSCS"){
				this->FirstComeFirstServe(1, 0, 0, current_time);
			}
			if(this->algorithm == "SJS"){

			}
			if(this->algorithm == "SRTS"){

			}
			if(this->algorithm == "RR"){

			}
			if(this->algorithm == "PS"){

			}
			if(this->algorithm == "MLQ"){

			}
			if(this->algorithm == "MLFQ"){

			}
		}

		void Run(int current_time){
			this->Insert(current_time);
			if(this->type == "Common"){
				this->CommonCore(current_time);
			}
			if(this->type == "Single"){
				this->SingleCore(current_time);
			}
		}
		
};

