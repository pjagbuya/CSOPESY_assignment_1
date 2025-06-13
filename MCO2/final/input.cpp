#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
using namespace std;
#define ARR_SIZE 100

#include "screen.h"

class Input : public Screen {
	private:
		string input;
		bool is_command_done;
		vector<string> cli_list;
		int cli_index;
        vector<string> suggest_command;
		int suggest_index;
		int suggest_key;
		vector<vector<string>> suggest_list;

	public:
		Input(int max_height) : Screen("Input", max_height){
			this->input = "";
			this->is_command_done = false;
			this->cli_index = 0;
		}

		//Getters and Setters

		void SetInput(string input) override { this->input = input; }
		string GetInput() override { return this->input; }
		void SetCommandDone(bool is_command_done) override { this->is_command_done = is_command_done; }
		bool GetCommandDone() override { return this->is_command_done; }
		void SetCliList(vector<string> cli_list) override { this->cli_list = cli_list; }
		vector<string> GetCliList() override { return this->cli_list; }

		// Other Methods

        void ClearCliList() override { this->cli_list.clear(); this->cli_index = 0; }

        // CLI Methods

		void KeyInput() override {
	
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
		}

		void Decoder(smatch* match, int* action) override {
			
			vector<regex> command_list = {
				//invalid						//-1
				(regex)"(clear)", 				//0
				(regex)"(exit)", 				//1
				(regex)"(screen -s (\\S+))",	//2
				(regex)"(screen -r (\\S+))",	//3
                (regex)"(screen -ls)",			//4
				(regex)"(ping)",				//5
			};

			bool is_valid = false;
			
			if(this->is_command_done){
				
				for(int i = 0; i < command_list.size(); i++){
					if(regex_match(this->input, *match, command_list[i])){
						is_valid = true;
						*action = i;
						i = command_list.size(); 
					}
				}
				if(!is_valid){
					*action = -1;
				}
				
				this->input = "";
			}
		}

		void Push(string input) override {
			if(this->cli_list.size() == 0){
				this->cli_list.push_back(input);
			}
			else{
				if(input == "Invalid Command Line"){
					if(this->cli_list[this->cli_list.size() - 1] == "Invalid Command Line"){
						this->cli_list[this->cli_list.size() - 1] = input;
					}
					else{
						this->cli_list.push_back(input);
					}
				}
				else{
					if(this->cli_list[this->cli_list.size() - 1] != "Invalid Command Line"){
						if(this->cli_list.size() >= this->GetMaxHeight() - 1){
							this->cli_list[this->cli_list.size() - 1] = input;
						}
						else{
							this->cli_list.push_back(input);
						}
					}
					else{
						this->cli_list[this->cli_list.size() - 1] = input;
					}
				}
			}
		}

		void AssembleOutputList() override {
            this->SetLine("Enter command: " + this->input, 0);
			for(int i = 0; i < this->cli_list.size(); i++){
				this->SetLine(this->cli_list[i], i + 1);
			}

			this->SetOutputSize(this->cli_list.size() + 1);
		}
};