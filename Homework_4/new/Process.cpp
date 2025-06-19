#include "Lib.h"

#include "Screen.cpp"
#include "File.cpp"

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
        string status;

    public:
        Process() : screen(), file() {}

        void Initialize() {
            this->status = "Not Ready";
            this->screen.Initialize(pid);
        }

        string GetStatus(){ return this->status; }

        void ProcessTerminate(){
            if(this->burst_time == 0){
                this->Terminate();
            }
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
                this->screen.Run(this->print_output);
                this->burst_time--;
            }
        }

        void Terminate() {
            this->Report();
        }

        void Report() {
        }
};