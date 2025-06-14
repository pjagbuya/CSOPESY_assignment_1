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
		vector<string> print_list;
		string name;
		time_t time_created;
		int max_height;
		int output_size;
		int current_line;
		int max_line;

	public:
		Screen(string name, int max_height){
			this->name = name;
			time_t timestamp;
			time(&timestamp);
			this->time_created = timestamp;
			this->max_height = max_height;
			this->output_size = 0;
			this->current_line = 0;
			this->max_line = max_height;
		}

		void Initialize(){
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
		void SetOutputSize(int output_size){ this->output_size = output_size; }
		int GetOutputSize(){ return this->output_size; }

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
			"Time Created: " + time_string,
			};
			
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}

			this->output_size = header.size();
		}

		void ClearOutputList(){
			for(int i = 0; i < this->output_list.size(); i++){
				this->output_list[i] = " ";
			}
		}

		// input.cpp
		virtual void SetInput(string input) {}
		virtual string GetInput() {}

		virtual void SetCommandDone(bool is_command_done) {}
		virtual bool GetCommandDone() {}

		virtual void KeyInput() {}
		virtual void Decoder(smatch* match, int* action) {}
		virtual void Push(string input) {}

};

#endif // SCREEN_H