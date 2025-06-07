#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "process.h"

class ReadyQueue {
    private:
        int id;
        vector<Process> processes; 
        string algorithm;
        int quantum_time;
        int priority_level;

    public:
        ReadyQueue(int id, string algorithm, int quantum_time, int priority_level) {
            this->id = id;
            this->algorithm = algorithm;
            this->quantum_time = quantum_time;
            this->priority_level = priority_level;
        }

        int GetID(){ return this->id; }
        void SetID( int id ){ this->id = id; }
        vector<Process> GetProcesses() { return processes; }
        void SetProcesses(vector<Process> processes) { this->processes = processes; }
        string GetAlgorithm() { return algorithm; }
        void SetAlgorithm(string algorithm) { this->algorithm = algorithm; }
        int GetQuantumTime() { return quantum_time; }
        void SetQuantumTime(int quantum_time) { this->quantum_time = quantum_time; }
        int GetPriorityLevel() { return priority_level; }
        void SetPriorityLevel(int priority_level) { this->priority_level = priority_level; }

        void AddProcess(Process process) {
            this->processes.push_back(process);
        }

        Process FetchProcess(int index){
            if(index < this->processes.size()){
                return this->processes[index];
            }
        }

        Process RemoveProcess(int index){
            auto it = find(this->processes.begin(), this->processes.end(), this->processes[index]);
			if (it != this->processes.end()) {
				this->processes.erase(it);
			}
        }

        int GetProcessBurstTime(int index){
            if(index < this->processes.size()){
                return this->processes[index].GetBurstTime();
            }
        }

        int GetProcessPriority(int index){
            if(index < this->processes.size()){
                return this->processes[index].GetPriority();
            }
        }
};