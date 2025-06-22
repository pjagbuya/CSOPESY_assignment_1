#include "Lib.h"
#pragma once

#include "CommandInterpreter.h"

class Input {
    private:
        CommandInterpreter command_interpreter;
        string input;
        bool is_command_done;
        vector<string> cli_list;

    public:
        Input() : command_interpreter(CommandInterpreter()) {
            this->input = "";
            this->is_command_done = false;
        }

        const CommandInterpreter& GetCommandInterpreter() { return this->command_interpreter; }
        void SetCommandInterpreter(CommandInterpreter command_interpreter) { this->command_interpreter = command_interpreter; }
        string GetInput() { return this->input; }
        void SetInput(string input) { this->input = input; }
        bool IsCommandDone() { return this->is_command_done; }
        void SetCommandDone(bool is_command_done) { this->is_command_done = is_command_done; }

        void ResetCommandInterpreter() {
            this->command_interpreter.SetCommandExecuted(true);
            this->command_interpreter.SetAction(-2);
        }

        void Initialize() {
            this->command_interpreter.Initialize();
            this->input = "";
            this->is_command_done = false;
        }
        
        void ClearCliList() {
            this->cli_list.clear();
        }

        vector<string> GetCliList() {
            return this->cli_list;
        }

        void Push() {
            if(this->cli_list.back() == "Invalid Command Line") {
                this->cli_list.back() = this->input;
            }
            else {
                if(this->cli_list.size() >= 10) {
                    this->cli_list.pop_back();
                }
                this->cli_list.push_back(this->input);
            }
        }

        void Run(int screen_id = -1) {
            if(_kbhit()){
				char key = _getch();
                if (key == '\r' || key == '\n') {
                    this->is_command_done = true;
                }
                else if(key == 8){
                    if((this->input).length() != 0){
                        this->input.pop_back();
                    }
                }
                else{
                    this->input += key;
                }
			}
            
            if(this->is_command_done) {
                this->command_interpreter.Run(this->input, screen_id);
                this->input = "";
                this->is_command_done = false;
            }
        }
};