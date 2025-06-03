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
        vector<Process> processes; 
        string algorithm;
        int quantum_time;
        int priority_level;

    public:
        ReadyQueue(string algorithm, int quantum_time, int priority_level) {
            this->algorithm = algorithm;
            this->quantum_time = quantum_time;
            this->priority_level = priority_level;
        }

        vector<Process> GetProcesses() { return processes; }
        void SetProcesses(vector<Process> processes) { this->processes = processes; }
        string GetAlgorithm() { return algorithm; }
        void SetAlgorithm(string algorithm) { this->algorithm = algorithm; }
        int GetQuantumTime() { return quantum_time; }
        void SetQuantumTime(int quantum_time) { this->quantum_time = quantum_time; }
        int GetPriorityLevel() { return priority_level; }
        void SetPriorityLevel(int priority_level) { this->priority_level = priority_level; }

        void AddProcess(Process process) {
            processes.push_back(process);
        }
};