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

#ifndef SCREEN_H
#define SCREEN_H

class Screen {
	private:
		vector<string> output_list;
		string name;
		time_t time_created;
		int max_height;
		int max_width;
		int output_size;
		int current_line;
		int max_line;

		string input;
		bool is_command_done;
		bool is_printing;
		string print_str;
		bool is_command_quit;

		vector<string> screen_output_list;

	public:
		Screen(string name, int max_height, int max_width){
			this->name = name;
			time_t timestamp;
			time(&timestamp);
			this->time_created = timestamp;
			this->max_height = max_height;
			this->max_width = max_width;
			this->output_size = 0;
			this->current_line = 0;
			this->max_line = 1000;

			this->is_command_done = false;
			this->is_printing = false;
			this->print_str = "";
		}

		void Initialize(){
			this->output_list.resize(this->max_height);
			for(int i = 0; i < this->max_height; i++){
				this->output_list[i] = " ";
			}
			this->AssembleOutputList();
		}

		void Initialize(int max_height, int max_width){
			this->max_height = max_height;
			this->max_width = max_width;
			this->output_list.resize(this->max_height);
			for(int i = 0; i < this->max_height; i++){
				this->output_list[i] = " ";
			}
			this->AssembleOutputList();
		}
		
		void SetOutputList(vector<string> output_list){ this->output_list = output_list; }
		vector<string> GetOutputList(){ return this->output_list; }

		void SetName(string name){ this->name = name; }
		string GetName(){ return this->name; }

		void SetTimeCreated(time_t time_created){ this->time_created = time_created; }
		time_t GetTimeCreated(){ return this->time_created; }

		void SetMaxHeight(int max_height){ this->max_height = max_height; }
		int GetMaxHeight(){ return this->max_height; }

		void SetMaxWidth(int max_width){ this->max_width = max_width; }
		int GetMaxWidth(){ return this->max_width; }

		void SetOutputSize(int output_size){ this->output_size = output_size; }
		int GetOutputSize(){ return this->output_size; }

		void SetCurrentLine(int current_line){ this->current_line = current_line; }
		int GetCurrentLine(){ return this->current_line; }

		void SetMaxLine(int max_line){ this->max_line = max_line; }
		int GetMaxLine(){ return this->max_line; }

		void SetIsCommandDone(bool is_command_done){ this->is_command_done = is_command_done; }
		bool GetIsCommandDone(){ return this->is_command_done; }

		void SetIsPrinting(bool is_printing){ this->is_printing = is_printing; }
		bool GetIsPrinting(){ return this->is_printing; }

		void SetPrintStr(string print_str){ this->print_str = print_str; }
		string GetPrintStr(){ return this->print_str; }

		void SetIsCommandQuit(bool is_command_quit){ this->is_command_quit = is_command_quit; }
		bool GetIsCommandQuit(){ return this->is_command_quit; }

		void SetScreenOutputList(vector<string> screen_output_list){ this->screen_output_list = screen_output_list; }
		vector<string> GetScreenOutputList(){ return this->screen_output_list; }

		void SetLine(string str, int i){
			this->output_list[i] = str;
		}

		virtual void AssembleOutputList() {
			time_t time = this->GetTimeCreated();
			char* time_str = ctime(&time);
			string time_string(time_str);
			time_string.erase(time_string.length() - 1);
			
			vector<string> header = {
			"Screen Name: " + this->GetName(),
			"Line: " + to_string(this->current_line) + "/" + to_string(this->max_line),
			"Screen Time: " + time_string,
			"Input: " + this->input,
			};
			
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}

			if(this->screen_output_list.size() > 0){
				this->SetLine(this->screen_output_list[this->screen_output_list.size()-1], 4);
			}

			this->output_size = header.size();
		}

		void ClearOutputList(){
			for(int i = 0; i < this->output_list.size(); i++){
				this->output_list[i] = " ";
			}
		}

		void ClearScreenOutputList(){
			this->screen_output_list.clear();
			this->current_line = 0;
		}

		virtual void KeyInput() {
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

		virtual void Decoder(smatch* match, int* action) {}

		virtual void Decoder() { 

			smatch match;
			int action; 

			vector<regex> command_list = {
				//invalid						//-1
				(regex)"(clear)", 				//0
				(regex)"(exit)", 				//1
				(regex)"(print (\\S+))",		//2
				(regex)"(stop)", 				//3
			};

			bool is_valid = false;
			
			if(this->is_command_done){
				
				for(int i = 0; i < command_list.size(); i++){
					if(regex_match(this->input, match, command_list[i])){
						is_valid = true;
						action = i;
						i = command_list.size(); 
					}
				}
				if(!is_valid){
					action = -1;
				}
				
				this->input = "";

				switch(action){
					case 0:
						this->ClearScreenOutputList();
						this->screen_output_list.clear();
						break;
					case 1:
						this->screen_output_list.clear();
						break;
					case 2:
						this->print_str = match[2].str();
						this->is_printing = true;
						break;
					case 3:
						this->is_printing = false;
						break;
					default:
						break;
				}
			}
		}

		void Run(){
			if(this->is_printing){
				if(this->current_line < this->max_line){
					this->screen_output_list.push_back(this->current_line + " | " + this->print_str);
					this->current_line++;
				}
				else{
					this->output_list.erase(this->output_list.begin());
					this->screen_output_list.push_back(this->current_line + " | " + this->print_str);
				}
			}
		}
		
		// input.cpp
		virtual void SetInput(string input) {}
		virtual string GetInput() {}

		virtual void SetCommandDone(bool is_command_done) {}
		virtual bool GetCommandDone() {}

		virtual void SetCliList(vector<string> cli_list) {}
		virtual vector<string> GetCliList() {}

		virtual void SetSuggestCommand(vector<string> suggest_command) {}
		virtual vector<string> GetSuggestCommand() {}

		virtual void SetSuggestIndex(int suggest_index) {}
		virtual int GetSuggestIndex() {}

		virtual void SetSuggestKey(int suggest_key) {}
		virtual int GetSuggestKey() {}

		virtual void SetSuggestList(vector<vector<string>> suggest_list) {}
		virtual void SetSuggestList(int index, vector<string> suggest_list) {}
		virtual vector<vector<string>> GetSuggestList() {}

		virtual void ClearCliList() {}
		virtual void AddSuggestList(int index, string suggest) {}
		virtual void RemoveSuggestList(int index, string suggest) {}

		virtual void Push(string input) {}

};

#endif // SCREEN_H