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
				Process("P0", 0, 0, 5, 2, "A"),
				Process("P1", 1, 1, 3, 1, "B"),
				Process("P2", 2, 9, 8, 3, "C"),
				Process("P3", 3, 3, 6, 2, "D"),
				Process("P4", 4, 5, 2, 4, "E"),
				Process("P5", 5, 5, 4, 1, "F"),
				Process("P6", 6, 2, 7, 2, "G"),
				Process("P7", 7, 1, 1, 5, "H"),
				Process("P8", 8, 9, 9, 3, "I"),
				Process("P9", 9, 6, 2, 2, "J")
			};
		}

		void RemoveProcessFromInitial(int process_id) {
			initial_queue.erase(
				remove_if(initial_queue.begin(), initial_queue.end(),
					[process_id](Process process) {
						return process.GetID() == process_id;
					}),
				initial_queue.end()
			);
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

		void Arrive(Process process, int ready_queue_id){
			this->ready_queues[ready_queue_id].AddProcess(process);
		}

		bool IsFinished(){
			return false;
		}

		void AssignProcess(int core_id, int ready_queue_id, int process_id, int current_time){
			this->cores[core_id].SetProcess(this->ready_queues[ready_queue_id].FetchProcess(process_id));
			this->cores[core_id].GetProcess().StartProcess(current_time);
			this->ready_queues[ready_queue_id].RemoveProcess(process_id);
		}

		void ContextSwitch(int core_id, int ready_queue_id, int process_id, int current_time){
			this->cores[core_id].GetProcess().HaltProcess();
			this->ready_queues[ready_queue_id].AddProcess(this->cores[core_id].GetProcess());
			this->AssignProcess(core_id, ready_queue_id, process_id, current_time);
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

