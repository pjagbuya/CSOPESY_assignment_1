#include "Lib.h"

#include "CommandInterpreter.cpp"

class Input {
    private:
        CommandInterpreter command_interpreter;
        string input;
        bool is_command_done;

    public:
        Input() : command_interpreter(CommandInterpreter()) {
            this->input = "";
            this->is_command_done = false;
        }

        CommandInterpreter GetCommandInterpreter() { return this->command_interpreter; }
        void SetCommandInterpreter(CommandInterpreter command_interpreter) { this->command_interpreter = command_interpreter; }
        string GetInput() { return this->input; }
        void SetInput(string input) { this->input = input; }
        bool IsCommandDone() { return this->is_command_done; }
        void SetCommandDone(bool is_command_done) { this->is_command_done = is_command_done; }

        void Initialize() {
            this->command_interpreter.Initialize();
            this->input = "";
            this->is_command_done = false;
        }

        void Run() {
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
                this->command_interpreter.Run(this->input);
                this->input = "";
                this->is_command_done = false;
            }
        }
};