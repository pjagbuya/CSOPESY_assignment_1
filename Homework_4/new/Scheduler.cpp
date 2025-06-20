#include "Lib.h"

#include "CPU.cpp"
#include "Queue.cpp"

class Scheduler {
    private:
        string algo;
        CPU cpu;
        vector<Queue> ready_queue;
        Queue waiting_queue;
        Queue finished_queue;
        int cpu_cycle;

    public:
        Scheduler() : cpu(), ready_queue(), waiting_queue(), finished_queue() {}

        void Initialize(string algo, int queue_count, int core_count) { 
            this->algo = algo;
            cpu.Initialize(core_count);
            for (int i = 0; i < queue_count; ++i) {
                ready_queue.push_back(Queue());
            }
            cpu_cycle = 0;

        }

        void Run() {
            if (algo == "FCFS") {
                for(int i = 0; i < this->cpu.GetCores().size(); i++){
                    if(this->cpu.GetCore(i).GetProcess()->GetStatus() == "Terminated" && !this->ready_queue.empty()) {
                        this->cpu.GetCore(i).SetProcess(this->ready_queue[0].Get());
                    }
                }

            } 
            else if (algo == "RR") {

            }
            cpu_cycle++;       
        }
};