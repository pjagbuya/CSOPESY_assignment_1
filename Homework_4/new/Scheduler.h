#include "Lib.h"
#pragma once

#include "CPU.h"
#include "Queue.h"


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

        void SetCPU(CPU cpu) {
            this->cpu = cpu;
        }
        CPU GetCPU() {
            return this->cpu;
        }
        void SetReadyQueue(vector<Queue> ready_queue) {
            this->ready_queue = ready_queue;
        }
        vector<Queue> GetReadyQueue() {
            return this->ready_queue;
        }
        void SetWaitingQueue(Queue waiting_queue) {
            this->waiting_queue = waiting_queue;
        }
        Queue GetWaitingQueue() {
            return this->waiting_queue;
        }
        void SetFinishedQueue(Queue finished_queue) {
            this->finished_queue = finished_queue;
        }
        Queue GetFinishedQueue() {
            return this->finished_queue;
        }
        void PushReadyQueue(int index, shared_ptr<Process> process) {
            if (process) {
                this->ready_queue[index].Push(process);
            }
        }
        void PushWaitingQueue(shared_ptr<Process> process) {
            if (process) {
                this->waiting_queue.Push(process);
            }
        }
        void PushFinishedQueue(shared_ptr<Process> process) {
            if (process) {
                this->finished_queue.Push(process);
            }
        }

        void Initialize(string algo, int queue_count, int core_count) { 
            this->algo = algo;
            this->cpu.Initialize(core_count);
            for (int i = 0; i < queue_count; ++i) {
                this->ready_queue.push_back(Queue());
            }
            this->cpu_cycle = 0;

        }

        void Run() {
            if (algo == "FCFS") {
                for(int i = 0; i < this->cpu.GetCores().size(); i++){
                    
                    //cout << "Test0.1" << endl; Sleep(50);

                    //cout << this->cpu.GetCore(i).GetProcess();

                    //cout << "Test0.2" << endl; Sleep(50);

                    //cout << (this->ready_queue[0].GetSize() > 0);

                    //cout << "Test0.3" << endl; Sleep(50);

                    if(!this->cpu.GetCore(i).GetProcess() && this->ready_queue[0].GetSize() > 0) {

                        //cout << "Test1.1" << endl; Sleep(50);

                        this->cpu.GetCore(i).SetProcess(this->ready_queue[0].Get());

                        //cout << "Test1.2" << endl; Sleep(50);

                        this->cpu.GetCore(i).StartProcess();
                    }
                    else if(this->cpu.GetCore(i).GetProcess()){
                        if(this->cpu.GetCore(i).GetProcess()->GetStatus() == "Terminated" && this->ready_queue[0].GetSize() > 0) {
                            
                            //cout << "Test2.1" << endl; Sleep(50);
                            
                            this->finished_queue.Push(this->cpu.GetCore(i).GetProcess());

                            //cout << "Test2.2" << endl; Sleep(50);

                            this->cpu.GetCore(i).SetProcess(this->ready_queue[0].Get());

                            //cout << "Test2.3" << endl; Sleep(50);

                            this->cpu.GetCore(i).StartProcess();

                            //cout << "Test2.4" << endl; Sleep(50);
                        }
                    }
                }

            } 
            else if (algo == "RR") {

            }
            this->cpu_cycle++;       
        }
};