#include "Lib.h"

#include "ConsoleManager.cpp"
#include "Scheduler.cpp"
#include "Input.cpp"
#include "Output.cpp"

class OperatingSystem {
    private:
        ConsoleManager console_manager;
        Scheduler scheduler;
        Input input;
        Output output;
        bool is_initialized;
        bool is_quit;

    public:
        OperatingSystem() : console_manager(), scheduler(), input(), output() {
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
            } else if(input == "initialize") {
                this->is_initialized = true;
                this->Initialize();
            } else {
                cout << "Invalid command. Please type 'initialize' to start or 'exit' to exit." << endl;
            }
        }

        void Initialize() {
            this->console_manager.Initialize();
            this->scheduler.Initialize();
            this->input.Initialize();
            this->output.Initialize();
        }

        void Run(){

        }

        void KeyInput() {
            this->input.Run();
            
        }

        void AssembleOutput() {
            this->output.Run();
        }

        void ConsoleManage(){
            this->console_manager.Run();
        }
        
        void Schedule() {
            this->scheduler.Run();
        }

        void Shutdown() {
            this->console_manager.Shutdown();
            this->output.Shutdown();
        }

};