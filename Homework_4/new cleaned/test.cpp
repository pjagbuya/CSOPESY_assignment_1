#include "Lib.h"
#include <cstdlib>
#include "Scheduler.h"

HANDLE h_console;

void MoveCursorTo(COORD coord) { SetConsoleCursorPosition(h_console, coord); }
void ClearCurrentLine() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count_written;
    
    GetConsoleScreenBufferInfo(h_console, &csbi);
    COORD line_start = { 0, csbi.dwCursorPosition.Y };
    FillConsoleOutputCharacter(h_console, ' ', csbi.dwSize.X, line_start, &count_written);
}

void ClearLine(int line_number) {
    COORD coord = { 0, (SHORT)line_number };
    MoveCursorTo(coord);
    ClearCurrentLine();
}

void Initialize() {
    h_console = GetStdHandle(STD_OUTPUT_HANDLE);
}

void Run(vector<string> output_list) {
    int i = 0;
    
    for (string  line : output_list) {
        ClearLine(i);
        cout << line << endl;
        i++;
    }
        }

int main(){
    Scheduler scheduler;
    scheduler.Initialize("RR", 1, 4);
    Initialize();
    
    vector<string> output_list;

    bool is_command_done = false;
    string input = "";
    smatch match;
    int process_i = 0;

    while(true){
        scheduler.Run();

        output_list.clear();
        output_list.push_back("Waiting Processes: + " + to_string(scheduler.GetReadyQueue()[0].GetSize()));
        for (int i = 0; i < scheduler.GetReadyQueue()[0].GetSize(); i++) {
            output_list.push_back(scheduler.GetReadyQueue()[0].GetProcessAtIndex(i)->ProcessInfo());
        };
        output_list.push_back(" ");
        output_list.push_back("Running Processes: +  " + to_string(scheduler.GetCPU().GetCores().size()));
        for (int i = 0; i < scheduler.GetCPU().GetCores().size(); i++) {
            shared_ptr<Process> process = scheduler.GetCPU().GetCoreAtIndex(i).GetProcess();
            if (process) {
                output_list.push_back(process->ProcessInfo());
            } else {
                output_list.push_back("Core " + to_string(i) + " is idle.");
            }
        };
        output_list.push_back(" ");
        output_list.push_back("Finished Processes: +  " + to_string(scheduler.GetFinishedQueue().GetSize()));
        for (int i = 0; i < scheduler.GetFinishedQueue().GetSize(); i++) {
            output_list.push_back(scheduler.GetFinishedQueue().GetProcessAtIndex(i)->ProcessInfo());
        };

        if(_kbhit()){
            char key = _getch();
            if (key == '\r' || key == '\n') {
                is_command_done = true;
            }
            else if(key == 8){
                if((input).length() != 0){
                    input.pop_back();
                }
            }
            else{
                input += key;
            }
        }

        output_list.push_back("C:user>" + input);
        for (int i = 0; i < 8; i++) {
            output_list.push_back("");
        }
        if(is_command_done) {
            if(regex_match(input, match, (regex)"(create (\\d+))")){
                for(int i = 0; i < stoi(match[2]); i++) {
                        shared_ptr<Process> process = make_shared<Process>("screen_" + to_string(process_i), "SRTF then RR at Time Slice " + to_string(process_i), 100 + rand() % 2000, 10);
                        scheduler.PushReadyQueue(0, process);
                        process_i++;
                    }
            }
            input = "";
            is_command_done = false;
        }

        Run(output_list);
        Sleep(SLEEP_TIME);
    }
    
    return 0;
}