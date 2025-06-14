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
			this->suggest_index = -1;	
			this->suggest_key = 0;
            this->suggest_command = {
                "",
                "screen -r ",
            };

			vector<string> suggestions_0 = {
				"ping",
				"screen -s",
                "screen -r",
				"screen -ls",
				"clear",
				"exit",
				"",
			};

			vector<string> suggestions_1 = {
				"",
			};

			this->suggest_list.push_back(suggestions_0);
			this->suggest_list.push_back(suggestions_1);
		}

		//Getters and Setters

		void SetInput(string input) override { this->input = input; }
		string GetInput() override { return this->input; }
		void SetCommandDone(bool is_command_done) override { this->is_command_done = is_command_done; }
		bool GetCommandDone() override { return this->is_command_done; }
		void SetCliList(vector<string> cli_list) override { this->cli_list = cli_list; }
		vector<string> GetCliList() override { return this->cli_list; }
		void SetSuggestCommand(vector<string> suggest_command) override { this->suggest_command = suggest_command; }
        vector<string> GetSuggestCommand() override { return this->suggest_command; }
		void SetSuggestIndex(int suggest_index) override { this->suggest_index = suggest_index; }
		int GetSuggestIndex() override { return this->suggest_index; }
        void SetSuggestKey(int suggest_key) override { this->suggest_key = suggest_key; }
        int GetSuggestKey() override { return this->suggest_key; }
		void SetSuggestList(vector<vector<string>> suggest_list) override { this->suggest_list = suggest_list; }
		void SetSuggestList(int index, vector<string> suggest_list) override { this->suggest_list[index] = suggest_list; }
		vector<vector<string>> GetSuggestList() override { return this->suggest_list; }

		// Other Methods

        void ClearCliList() override { this->cli_list.clear(); this->cli_index = 0; }
		void AddSuggestList(int index, string suggest) override { 
			this->suggest_list[index].push_back(suggest);
		}
		void RemoveSuggestList(int index, string suggest) {
			auto it = find(this->suggest_list[index].begin(), this->suggest_list[index].end(), suggest);
			if (it != this->suggest_list[index].end()) {
				this->suggest_list[index].erase(it);
			}
		}

        // CLI Methods

		void Suggest(){
			if(this->suggest_index == -1){
				for(int i = 0; i < this->suggest_command.size(); i++){
					if(this->input == this->suggest_command[i]){
						this->suggest_index = i;
						this->suggest_key = 0;
						this->input = this->suggest_command[this->suggest_index] + this->suggest_list[this->suggest_index][this->suggest_key];
					}
				}
			}
			else{
				this->suggest_key++;
				if(this->suggest_key >= this->suggest_list[this->suggest_index].size()){
					this->suggest_key = 0;
				}
				this->input = this->suggest_command[this->suggest_index] + this->suggest_list[this->suggest_index][this->suggest_key];
			}
		}

		void KeyInput() override {
	
			if(_kbhit()){
				char key = _getch();
                if(key == '\t') {
                    this->Suggest();
                }
                else{
                    this->suggest_index = -1;
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