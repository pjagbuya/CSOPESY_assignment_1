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

	public:
		Screen(string name, int max_height, int max_width){
			this->name = name;
			time_t timestamp;
			time(&timestamp);
			this->time_created = timestamp;
			this->max_height = max_height;
			this->max_width = max_width;
		}

		virtual void Initialize(){
			this->output_list.resize(this->max_height);
			for(int i = 0; i < this->max_height; i++){
				this->output_list[i] = " ";
			}
			this->AssembleOutputList();
		}

		virtual void Initialize(int max_height, int max_width){
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

		void SetLine(string str, int i){
			this->output_list[i] = str;
		}

		virtual void AssembleOutputList() {
			time_t time = this->GetTimeCreated();
			char* time_str = ctime(&time);
			string time_string(time_str);
			
			vector<string> header = {
			"Screen Name: " + this->GetName(),
			//"Screen Time: " + time_string,
			"Screen displaying... Type screen -r to remove",
			"Time Size: " + to_string(time_string.length())
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
		virtual string GetInput() { return ""; }

		virtual void SetCommandDone(bool is_command_done) {}
		virtual bool GetCommandDone() { return false; }

		virtual void SetOutput(string output) {}
		virtual string GetOutput() { return ""; }

		virtual void SetMoveDone(bool is_move_done) {}
		virtual bool GetMoveDone() { return false; }
		
		virtual void SetMove(int move) {}
		virtual int GetMove() { return 0; }

		virtual void ClearCliList() {}

		virtual void KeyInput() {}
		virtual void Decoder(int* action) {}
		virtual void Push(string input) {}

		// galaga.cpp
		virtual void Play(int move, int clock_cycle){}
};

#endif // SCREEN_H