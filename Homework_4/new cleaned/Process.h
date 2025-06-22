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
        int current_burst_time;
        int burst_time;
        int current_time_quantum;
        int time_quantum;
        string status;
        string time_created;
        string time_finished;
        int core_id;

    public:
        Process() : screen(), file(), status("Not Ready"), core_id(-1) {
            this->pid = "";
            this->print_output = "";
            this->arrival_time = 0;
            this->turnaround_time = 0;
            this->completion_time = 0;
            this->response_time = 0;
            this->wait_time = 0;
            this->current_burst_time = 0;
            this->burst_time = 0;
            this->current_time_quantum = 0;
            this->time_quantum = 0;
            this->time_created = screen.GetTime();
        }

        Process(string pid, string print_output, int burst_time, int time_quantum) : screen(), file() {
            this->pid = pid;
            this->print_output = print_output;
            this->current_burst_time = burst_time;
            this->burst_time = burst_time;
            this->current_time_quantum = 0;
            this->time_quantum = time_quantum;
            this->status = "Ready";
            this->time_created = screen.GetTime();
            this->core_id = -1;

            this->arrival_time = 0;
            this->turnaround_time = 0;
            this->completion_time = 0;
            this->response_time = 0;
            this->wait_time = 0;
            this->time_finished = "";
        }

        const Screen& GetScreen() const { return this->screen; }
        void SetScreen(const Screen& screen) { this->screen = screen; }
        const File& GetFile() const { return this->file; }
        void SetFile(const File& file) { this->file = file; }
        string GetPID() { return this->pid; }
        void SetPID(string pid) { this->pid = pid; }
        string GetPrintOutput() { return this->print_output; }
        void SetPrintOutput(string print_output) { this->print_output = print_output; }
        int GetArrivalTime() { return this->arrival_time; }
        void SetArrivalTime(int arrival_time) { this->arrival_time = arrival_time; }
        int GetTurnaroundTime() { return this->turnaround_time; }
        void SetTurnaroundTime(int turnaround_time) { this->turnaround_time = turnaround_time; }
        int GetCompletionTime() { return this->completion_time; }
        void SetCompletionTime(int completion_time) { this->completion_time = completion_time; }
        int GetResponseTime() { return this->response_time; }
        void SetResponseTime(int response_time) { this->response_time = response_time; }
        int GetWaitTime() { return this->wait_time; }
        void SetWaitTime(int wait_time) { this->wait_time = wait_time; }
        int GetCurrentBurstTime() { return this->current_burst_time; }
        void SetCurrentBurstTime(int current_burst_time) { this->current_burst_time = current_burst_time; }
        int GetBurstTime() { return this->current_burst_time; }
        void SetBurstTime(int burst_time) { this->current_burst_time = burst_time; }
        int GetTimeQuantum() { return this->time_quantum; }
        void SetTimeQuantum(int time_quantum) { this->time_quantum = time_quantum; }
        string GetStatus() { return this->status; }
        void SetStatus(string status) { this->status = status; }
        string GetTimeCreated() { return this->time_created; }
        void SetTimeCreated(string time_created) { this->time_created = time_created; }
        int GetCoreId() { return this->core_id; }
        void SetCoreId(int core_id) { this->core_id = core_id; }

        void Initialize() {
            this->status = "Not Ready";
            this->screen.Initialize(pid);
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
            this->core_id = -1;
        }

        void Run() {
            if(this->status == "Run"){
                this->screen.Run(this->print_output);
                this->current_burst_time--;
                this->current_time_quantum++;
            }
            if(this->current_burst_time == 0){
                this->Terminate();
            }
            else if(this->current_time_quantum >= this->time_quantum && this->time_quantum > 0){
                this->Pause();
                this->current_time_quantum = 0;
            }
        }

        void Terminate() {
            this->status = "Terminated";
            this->time_finished = screen.GetTime();
            this->Report();
        }

        void Report() {
        }

        string ProcessInfo() {
            if(this->status != "Terminated"){
                return this->pid + "   (" + this->time_created + ")   " + to_string(this->core_id) + "   " + to_string(this->burst_time-this->current_burst_time) + "/" + to_string(this->burst_time);   
            }
            else{
                return this->pid + "   (" + this->time_finished + ")   " + "Finished" + "   " + to_string(this->burst_time-this->current_burst_time) + "/" + to_string(this->burst_time);   
            }
        }
};