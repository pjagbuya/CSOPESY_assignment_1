#include "Lib.h"
#pragma once

#include "Process.h"

class Queue {
    private:
        vector<shared_ptr<Process>> processes;

    public:
        Queue() {}

        void Push(shared_ptr<Process> process){
            if (process) {
                processes.push_back(process);
            }
        }

        int GetSize() {
            return processes.size();
        }

        shared_ptr<Process> GetProcessAtIndex(int index) {
            if (index < 0 || index >= processes.size()) {
                return nullptr;
            }
            return processes[index];
        }

        shared_ptr<Process> Get(){
            shared_ptr<Process> process = processes.empty() ? nullptr : processes.front();
            processes.erase(processes.begin());
            return process;
        }
};