#include "Lib.h"

class CommandInterpreter {
    private:
        bool is_initialized;
        bool is_command_executed;
        int action;
        smatch match;

    public:
        CommandInterpreter() {
            this->is_initialized = false;
            this->is_command_executed = true;
            this->action = -2;
        }

        bool IsInitialize() { return this->is_initialized; }
        void SetInitialize(bool is_initialized) { this->is_initialized = is_initialized; }
        bool IsCommandExecuted() { return this->is_command_executed; }
        void SetCommandExecuted(bool is_command_done) { this->is_command_executed = is_command_done; }
        int GetAction() { return this->action; }
        void SetAction(int action) { this->action = action; }
        smatch GetMatch() { return this->match; }
        void SetMatch(smatch match) { this->match = match; }

        void Initialize() {
            this->is_initialized = true;
            this->is_command_executed = true;
        }

        void Run(string input) {
            this->is_command_executed = false;
            vector<regex> command_list = {
				//invalid						//-1
				(regex)"(exit)", 				//0
				(regex)"(screen -s (\\S+))",	//1
				(regex)"(screen -r (\\S+))",	//2
                (regex)"(screen -ls)",			//3
                (regex)"(scheduler-start)",		//4
                (regex)"(scheduler-stop)",		//5
                (regex)"(report-util)",			//6
                (regex)"(ping)",				//7
				(regex)"(clear)",				//8
			};

			bool is_valid = false;
			
            for(int i = 0; i < command_list.size(); i++){
                if(regex_match(input, this->match, command_list[i])){
                    is_valid = true;
                    this->action = i;
                    i = command_list.size();
                }
            }
            if(!is_valid){
                this->action = -1;
            }
        }
};