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
		string message;

	public:
		Input(int max_height) : Screen("Input", max_height){
			this->input = "";
			this->is_command_done = false;
		}

		//Getters and Setters

		void SetInput(string input) override { this->input = input; }
		string GetInput() override { return this->input; }
		void SetCommandDone(bool is_command_done) override { this->is_command_done = is_command_done; }
		bool GetCommandDone() override { return this->is_command_done; }

        // CLI Methods

		void KeyInput() {
	
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

		void Decoder(smatch* match, int* action) {
			
			vector<regex> command_list = {
				//invalid						//-1
				(regex)"(clear)", 				//0
				(regex)"(exit)", 				//1
				(regex)"(ping)",				//2
				(regex)"(MOV ([\\[\\]A-Za-z0-9]+), ([\\[\\]A-Za-z0-9]+))",	//3
				(regex)"(ADD ([\\[\\]A-Za-z0-9]+), ([\\[\\]A-Za-z0-9]+))",	//4
				(regex)"(SUB ([\\[\\]A-Za-z0-9]+), ([\\[\\]A-Za-z0-9]+))",	//5
				(regex)"(XOR ([\\[\\]A-Za-z0-9]+), ([\\[\\]A-Za-z0-9]+))",	//6
				(regex)"(MWRITE ([\\[\\]A-Za-z0-9]+), ([\\[\\]A-Za-z0-9]+))", //8

			};

			bool is_valid = false;
			
			if(this->is_command_done){
				this->Push(this->input);
				
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
				this->is_command_done = false;
			}
		}

		void Push(string input) {
			this->message = input;
		}

		void AssembleOutputList() override {
            this->SetLine("Enter command: " + this->input, 0);
			this->SetLine(this->message, 1);
		}

		void Run(smatch* match, int* action){
			this->KeyInput();
			this->Decoder(match, action);
			this->AssembleOutputList();
		}
};