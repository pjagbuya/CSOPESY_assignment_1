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
		string output;
		bool is_move_done;
		int move;

	public:
		Input(int max_height, int max_width) : Screen("Input", max_height, max_width){
			this->input = "";
			this->is_command_done = false;
			this->output = "";
			this->is_move_done = true;
			this->move = 3;
		}

		//Getters and Setters

		void SetInput(string input) override { this->input = input; }
		string GetInput() override { return this->input; }

		void SetCommandDone(bool is_command_done) override { this->is_command_done = is_command_done; }
		bool GetCommandDone() override { return this->is_command_done; }

		void SetOutput(string output) override { this->output = output; }
		string GetOutput() override { return this->output; }

		void SetMoveDone(bool is_move_done) override { this->is_move_done = is_move_done; }
		bool GetMoveDone() override { return this->is_move_done; }

		void SetMove(int move) override { this->move = move; }
		int GetMove() override { return this->move; }

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
				else if (key == 72) {
					this->move = 0;
					//up
				}
				else if (key == 80) {
					this->move = 1;
					//down
				}
				else if (key == 75) {
					this->move = 2;
					//left
				}
				else if (key == 77) {
					this->move = 3;
					//right
				}
				else{
					this->input += key;
				}
			}
		}

		void Decoder(int* action) override {
			
			vector<regex> command_list = {
				//invalid						//-1
				(regex)"(pause)", 				//0
				(regex)"(resume)", 				//1
				(regex)"(upgrade)", 			//2
				(regex)"(heal)",				//3
			};

			bool is_valid = false;
			
			if(this->is_command_done){
				
				for(int i = 0; i < command_list.size(); i++){
					if(regex_match(this->input, command_list[i])){
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
	};
		