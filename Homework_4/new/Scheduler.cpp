#include "Lib.h"

#include "CPU.cpp"
#include "Queue.cpp"

class Scheduler {
    private:
        CPU cpu;
        vector<Queue> ready_queue;
        Queue waiting_queue;
        Queue finished_queue;

    public:
        Scheduler() : cpu(), ready_queue(), waiting_queue(), finished_queue() {}

        void Initialize() {
            cpu.Initialize();
            for (auto& queue : ready_queue) {
                queue.Initialize();
            }
            waiting_queue.Initialize();
            finished_queue.Initialize();
        }

        void Run() {
            
        }
};