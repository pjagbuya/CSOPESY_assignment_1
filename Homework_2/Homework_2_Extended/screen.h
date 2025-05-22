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

	public:
		Screen(string name, int max_height, int max_width){
			this->name = name;
			time_t timestamp;
			time(&timestamp);
			this->time_created = timestamp;
			this->max_height = max_height;
			this->max_width = max_width;
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

		void SetLine(string str, int i){
			this->output_list[i] = str;
		}

		virtual void AssembleOutputList() {
			time_t time = this->GetTimeCreated();
			char* time_str = ctime(&time);
			
			vector<string> header = {
			"Screen Name: " + this->GetName(),
			string("Screen Time: ") + time_str,
			};
			
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}
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

		virtual void KeyInput() {}
		virtual void Decoder(smatch* match, int* action) {}
		virtual void Push(string input) {}

};

#endif // SCREEN_H