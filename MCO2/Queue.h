#include "Lib.h"
#pragma once

#include "Process.h"

class Queue {
    private:
        vector<shared_ptr<Process>> processes;

    public:
        Queue() {}

        vector<shared_ptr<Process>> GetProcesses() { return this->processes; }
        void SetProcesses(vector<shared_ptr<Process>> processes) { this->processes = processes; }
        int GetSize() { return processes.size(); }

        void Push(shared_ptr<Process> process){
            if (process) {
                this->processes.push_back(process);
            }
        }

        shared_ptr<Process> GetProcessAtIndex(int index) {
            if (index < 0 || index >= processes.size()) {
                return nullptr;
            }
            return this->processes[index];
        }

        shared_ptr<Process> Get(){
            shared_ptr<Process> process = this->processes.empty() ? nullptr : this->processes.front();
            this->processes.erase(processes.begin());
            return process;
        }
};