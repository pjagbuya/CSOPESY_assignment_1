#include "Lib.h"
#pragma once

class CommandInterpreter {
    private:
        bool is_initialized;
        bool is_command_executed;
        int action;
        vector<string> match;

    public:
        CommandInterpreter() {
            this->is_initialized = false;
            this->is_command_executed = true;
            this->action = -2;
        }

        bool IsInitialize() { return this->is_initialized; }
        void SetInitialize(bool is_initialized) { this->is_initialized = is_initialized; }
        bool IsCommandExecuted() { return this->is_command_executed; }
        void SetCommandExecuted(bool is_command_executed) { this->is_command_executed = is_command_executed; }
        int GetAction() { return this->action; }
        void SetAction(int action) { this->action = action; }
        vector<string> GetMatch() { return this->match; }

        void Initialize() {
            this->is_initialized = true;
            this->is_command_executed = true;
        }

        void Run(string input, int screen_id = -1) {
            this->is_command_executed = false;
            vector<regex> command_list;

            switch(screen_id){
                case -1:
                    command_list = {
                        //invalid						//-1
                        (regex)"(exit)", 				//0
                        (regex)"(screen -s (\\S+))",	//1
                        (regex)"(screen -r (\\S+))",	//2
                        (regex)"(l)",			//3
                        (regex)"(create (\\d+))",		//4
                        (regex)"(scheduler-stop)",		//5
                        (regex)"(report-util)",			//6
                        (regex)"(ping)",				//7
                        (regex)"(clear)",				//8
                    };
                    break;
                default:
                    command_list = {
                        // invalid                      //-1
                        (regex)"(exit)",                //0
                        (regex)"(process_smi)",         //1
                        (regex)"(print (\\S+) (\\d+))"  //2
                    };
                    break;
            }

			bool is_valid = false;
			
            smatch match; 
            for(int i = 0; i < command_list.size(); i++){
                if(regex_match(input, match, command_list[i])){
                    for(int j = 0; j < match.size(); j++){
                        this->match.push_back(match[j]);
                    }
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