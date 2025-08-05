#include "Lib.h"
#pragma once

#include "Screen.h"
#include "File.h"
#include "PageTable.h"
#include "SymbolTable.h"

class Process {
    private:
        Screen screen;
        File file;
        PageTable page_table;
        string pid;
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

        int program_counter = 0; 
        vector<int> loop_stack;
        vector<int> loop_line_stack;
        vector<int> loop_counter_stack;
        int loop_stack_pointer = 0;
        int last_program_line = 0;

        SymbolTable symbol_table;

    public:
        Process() : screen(), file(), status("Not Ready"), core_id(-1) {
            this->pid = "";
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

        Process(string pid, string print_output, int num, int time_quantum) : screen(), file() {
            
            this->screen.Initialize(pid);
            this->file.Initialize(num);

            this->pid = pid;
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
            
            this->loop_stack.push_back(0);
            this->loop_counter_stack.push_back(0);
            this->loop_line_stack.push_back(0);

            this->Compile();
        }

        const Screen& GetScreen() const { return this->screen; }
        void SetScreen(const Screen& screen) { this->screen = screen; }
        const File& GetFile() const { return this->file; }
        void SetFile(const File& file) { this->file = file; }
        string GetPID() { return this->pid; }
        void SetPID(string pid) { this->pid = pid; }
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

        void Compile() {
            this->burst_time = this->file.GetBurstTime();
            this->current_burst_time = this->burst_time;

            this->last_program_line = this->file.GetFileSize();
            for(string line : this->file.GetFileContent()) {
                if(line.find("FOR")!= string::npos) {
                    smatch match;
                    regex re(R"(FOR\s+(\d+))");
                    if (regex_search(line, match, re)) {
                        int number = stoi(match[1]);
                        this->loop_stack.push_back(number);
                        this->loop_counter_stack.push_back(0);
                        this->loop_stack_pointer++;
                    }
                    this->loop_line_stack.push_back(this->program_counter);
                }
                this->program_counter++;
            }
            this->program_counter = 0;
        }

        void Ready(){
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
                this->InterpretLine(this->file.GetFileLine(this->program_counter));
                if(this->program_counter < this->last_program_line) {
                    this->program_counter++;
                }
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

        void InterpretLine(string line){
            vector<regex> code_list = {
                regex(R"(DECLARE\s+(\w+)(?:\s*=\s*(\d+))?)"),
                regex(R"((\w+)\s*=\s*(\d+))"),
                regex(R"(ADD\s+(\w+),\s+(\w+),\s+(\w+))"),
                regex(R"(FOR\s+(\d+))"),
                regex(R"(END)"),
                regex(R"(PRINT\s+(.*))")
            };

            string output = to_string(this->program_counter) + "  " + this->file.GetFileLine(this->program_counter) + "  |  ";
            smatch match;

            string dest, src1, src2;
            int val1, val2;
            int size;

            for(int i = 0; i < code_list.size(); i++){
                if(regex_match(line, match, code_list[i])){
                    switch(i){
                        case 0: // DECLARE
                            size = this->symbol_table.GetSize();
                            if(match[2].matched) {
                                this->symbol_table.Insert(size, string(match[1]), stoi(string(match[2])));
                            } else {
                                this->symbol_table.Insert(size, string(match[1]), 0);
                            }
                            break;
                        case 1: // VAR_ASSIGN
                            this->symbol_table.Update(string(match[1]), stoi(match[2]));
                            break;
                        case 2: // ADD
                            dest = string(match[1]);
                            src1 = string(match[2]);
                            src2 = string(match[3]);

                            val1 = regex_match(src1, regex(R"(\d+)")) ? stoi(src1) : this->symbol_table.Fetch(src1);
                            val2 = regex_match(src2, regex(R"(\d+)")) ? stoi(src2) : this->symbol_table.Fetch(src2);

                            this->symbol_table.Update(dest, val1 + val2);
                            break;
                        case 3: // FOR
                            this->loop_stack_pointer++;
                            this->loop_counter_stack[this->loop_stack_pointer] = stoi(match[1]);
                            this->loop_line_stack.push_back(this->program_counter);
                            break;
                        case 4: // END
                            if(this->loop_counter_stack[this->loop_stack_pointer] == 1){
                                this->loop_stack_pointer--;
                            }
                            else {
                                this->loop_counter_stack[this->loop_stack_pointer]--;
                                this->program_counter = this->loop_line_stack[this->loop_stack_pointer];
                            }
                            break;
                        case 5: { // PRINT
                            string print = match[1];

                            smatch str_match;
                            if (regex_match(print, str_match, regex(R"(^\'(.*)\'$)"))) {
                                print = str_match[1];
                            } 
                            else if (this->symbol_table.Fetch(print) != -1) {
                                print = to_string(this->symbol_table.Fetch(print));
                            } 
                            else {
                                print = "Undefined variable: " + print;
                            }

                            output += print;
                            break;
                        }

                    }
                    i = 999;
                }
            }

            this->screen.Run(output, this->core_id);
        }

        void Terminate() {
            this->status = "Terminated";
            this->time_finished = screen.GetTime();
            this->Report();
        }

        void Report() {
            this->screen.ReportUtil(this->pid);
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