#include "Lib.h"

#include "ConsoleManager.h"
#include "Scheduler.h"
#include "Input.h"
#include "Output.h"

class OperatingSystem {
    private:
        ConsoleManager console_manager;
        Scheduler scheduler;
        Input input;
        Output output;
        bool is_initialized;
        bool is_quit;
        Screen screen;

    public:
        OperatingSystem() : console_manager(), scheduler(), input(), output(), screen(){
            this->is_initialized = false;
            this->is_quit = false;
        }

        ConsoleManager GetConsoleManager() { return this->console_manager; }
        void SetConsoleManager(ConsoleManager console_manager) { this->console_manager = console_manager; }
        Scheduler GetScheduler() { return this->scheduler; }
        void SetScheduler(Scheduler scheduler) { this->scheduler = scheduler; }
        Input GetInput() { return this->input; }
        void SetInput(Input input) { this->input = input; }
        Output GetOutput() { return this->output; }
        void SetOutput(Output output) { this->output = output; }
        bool IsInitialized() { return this->is_initialized; }
        void SetInitialized(bool is_initialized) { this->is_initialized = is_initialized; }
        bool IsQuit() { return this->is_quit; }
        void SetQuit(bool is_quit) { this->is_quit = is_quit; }

        void PreBoot(){
            cout << "Operating System Pre-Booting..." << endl;
            string input;
            cin >> input;
            if (input == "exit") {
                this->is_quit = true;
            } else if(input == "a") {
                this->is_initialized = true;
                this->Initialize();
            } else {
                cout << "Invalid command. Please type 'initialize' to start or 'exit' to exit." << endl;
            }
        }

        void Initialize() {
            this->console_manager.Initialize();
            this->scheduler.Initialize("FCFS", 1, 4); // Example parameters for FCFS scheduling with 3 queues and 4 cores
            this->input.Initialize();
            this->output.Initialize();
            cout << "Operating System Initialized." << endl;
            Sleep(1000); // Simulate some delay for initialization
            system("cls");
        }

        void Run(){
            this->KeyInput();
            this->ConsoleManage();
            this->AssembleOutput(this->console_manager.GetOutputList());
            this->Schedule();
            Sleep(50);
        }

        void KeyInput() {
            this->input.Run(this->console_manager.GetCurrentScreenId());

            if(!this->input.GetCommandInterpreter().IsCommandExecuted()) {
                vector<string> output_list;
                switch(this->input.GetCommandInterpreter().GetAction()) {
                    case 0: // exit
                        this->is_quit = true;
                        break;
                    case 1: // screen -s
                        break;
                    case 2: // screen -r
                        break;
                    case 3: // screen -ls
                        output_list.clear();
                        this->screen.ClearOutputList();
                        output_list.push_back("------------------------------");
                        output_list.push_back("Waiting Processes: + " + to_string(this->scheduler.GetReadyQueue()[0].GetSize()));
                        for (int i = 0; i < this->scheduler.GetReadyQueue()[0].GetSize(); i++) {
                            output_list.push_back(this->scheduler.GetReadyQueue()[0].GetProcessAtIndex(i)->ProcessInfo());
                        };
                        output_list.push_back(" ");
                        output_list.push_back("Running Processes: +  " + to_string(this->scheduler.GetCPU().GetCores().size()));
                        for (int i = 0; i < this->scheduler.GetCPU().GetCores().size(); i++) {
                            shared_ptr<Process> process = this->scheduler.GetCPU().GetCore(i).GetProcess();
                            if (process) {
                                output_list.push_back(process->ProcessInfo());
                            } else {
                                output_list.push_back("Core " + to_string(i) + " is idle.");
                            }
                        };
                        output_list.push_back(" ");
                        output_list.push_back("Finished Processes: +  " + to_string(this->scheduler.GetFinishedQueue().GetSize()));
                        for (int i = 0; i < this->scheduler.GetFinishedQueue().GetSize(); i++) {
                            output_list.push_back(this->scheduler.GetFinishedQueue().GetProcessAtIndex(i)->ProcessInfo());
                        };
                        this->screen.SetOutputList(output_list);
                        break;
                    case 4: // create
                        for(int i = 0; i < stoi(this->input.GetCommandInterpreter()->GetMatch()[2]); i++) {
                            shared_ptr<Process> process = make_shared<Process>("screen_" + to_string(i), "SRTF then RR at Time Slice " + to_string(i), 100);
                            this->scheduler.PushReadyQueue(0, process);
                        }
                        this->screen.SetOutputList({"Processes created successfully.", "Waiting Processes: + " + to_string(this->scheduler.GetReadyQueue()[0].GetSize())});
            
                        break;
                    case 5: // scheduler-stop
                        break;
                    case 6: // report-util
                        break;
                    case 7: // ping
                        
                        break;
                    case 8: // clear
                        this->screen.ClearOutputList();
                        system("cls");
                        break;
                    default:
                        this->input.GetCommandInterpreter().SetAction(-2);
                        break;
                }
                this->input.ResetCommandInterpreter();
            }
            
        }

        void AssembleOutput(vector<string> output_list) {
            this->output.Run(output_list);
        }

        void ConsoleManage() {
            this->console_manager.Run(this->screen, this->input);
        }
        
        void Schedule() {
            this->scheduler.Run();
        }

        void Shutdown() {
            this->console_manager.Shutdown();
            this->output.Shutdown();
        }

};