#include "Lib.h"

#include "Process.cpp"

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

        shared_ptr<Process> Get(){
            shared_ptr<Process> process = processes.empty() ? nullptr : processes.front();
            processes.erase(processes.begin());
            return process;
        }
};