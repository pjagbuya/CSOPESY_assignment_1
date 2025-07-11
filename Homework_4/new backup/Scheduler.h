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
                    if(!this->cpu.GetCore(i).GetProcess() && this->ready_queue[0].GetSize() > 0) {
                        shared_ptr<Process> process = this->ready_queue[0].Get();
                        process->Start();
                        this->cpu.SetProcessToCore(i, process);
                        Sleep(SLEEP_TIME);
                    }
                    else if(this->cpu.GetCore(i).GetProcess()){
                        if(this->cpu.GetCore(i).GetProcess()->GetStatus() == "Terminated") {
                            this->finished_queue.Push(this->cpu.GetCore(i).GetProcess());
                            this->cpu.ClearProcessAtCore(i);
                        }
                    }
                }

            } 
            else if (algo == "RR") {
                for(int i = 0; i < this->cpu.GetCores().size(); i++){
                    if(!this->cpu.GetCore(i).GetProcess() && this->ready_queue[0].GetSize() > 0) {
                        shared_ptr<Process> process = this->ready_queue[0].Get();
                        process->Start();
                        this->cpu.SetProcessToCore(i, process);
                        Sleep(SLEEP_TIME);
                    }
                    else if(this->cpu.GetCore(i).GetProcess()){
                        if(this->cpu.GetCore(i).GetProcess()->GetStatus() == "Terminated") {
                            this->finished_queue.Push(this->cpu.GetCore(i).GetProcess());
                            this->cpu.ClearProcessAtCore(i);
                        }
                        else if(this->cpu.GetCore(i).GetProcess()->GetStatus() == "Interrupt") {
                            this->ready_queue[0].Push(this->cpu.GetCore(i).GetProcess());
                            this->cpu.ClearProcessAtCore(i);
                        }
                    }
                }
            }
            this->cpu_cycle++;       
        }
};