#include "Lib.h"
#pragma once

#include "Screen.h"
#include "File.h"

class Process {
    private:
        Screen screen;
        File file;
        string pid;
        string print_output;
        int arrival_time;
        int turnaround_time;
        int completion_time;
        int response_time;
        int wait_time;
        int burst_time;
        int full_burst_time;
        int time_quantum;
        int full_time_quantum;
        string status;
        string time_created;
        int core_id;

    public:
        Process() : screen(), file() {
        }

        Process(string pid, string print_output, int burst_time, int full_time_quantum) : screen(), file() {
            this->pid = pid;
            this->print_output = print_output;
            this->burst_time = burst_time;
            this->full_burst_time = burst_time;
            this->status = "Ready";
            this->time_created = screen.GetTime();
            this->core_id = -1; // Default core ID
            this->full_time_quantum = full_time_quantum;
        }

        string GetPID() { return this->pid; }

        void Initialize() {
            this->status = "Not Ready";
            this->screen.Initialize(pid);
        }

        string GetStatus(){ return this->status; }

        int GetBurstTime() { return this->burst_time; }

        int GetTimeQuantum() { return this->time_quantum; }
        
        void SetTimeQuantum(int time_quantum) {
            this->time_quantum = time_quantum;
        }

        void ProcessTerminate(){
            if(this->burst_time == 0){
                this->Terminate();
            }
        }

        void ProcessInterrupt(){
            if(this->time_quantum == this->full_time_quantum){
                this->Pause();
                this->time_quantum = 0;
            }
        }

        void SetCoreId(int core_id) {
            this->core_id = core_id;
        }

        void Ready(string print_output){
            this->print_output = print_output;
            this->status = "Ready";
        }

        void Start(){
            this->status = "Run";
        }

        void Pause(){
            this->status = "Interrupt";
        }

        void Run() {
            if(this->status == "Run"){
                //this->screen.Run(this->print_output);
                this->burst_time--;
                this->time_quantum++;
            }
            this->ProcessTerminate();
            this->ProcessInterrupt();
        }

        void Terminate() {
            this->status = "Terminated";
            this->Report();
        }

        void Report() {
        }

        string ProcessInfo() {
            if(this->status != "Terminated"){
                return this->pid + "   (" + this->time_created + ")   " + to_string(this->core_id) + "   " + to_string(this->full_burst_time-this->burst_time) + "/" + to_string(this->full_burst_time);   
            }
            else{
                return this->pid + "   (" + this->time_created + ")   " + "Finished" + "   " + to_string(this->full_burst_time-this->burst_time) + "/" + to_string(this->full_burst_time);   
            }
        }
};