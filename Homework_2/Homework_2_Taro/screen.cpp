#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
using namespace std;
#define ARR_SIZE 100

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
			"----------------------------------------",
			"Screen Name: " + this->GetName(),
			string("Screen Time: ") + time_str,
			"----------------------------------------",
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

};

class Header : public Screen {
	public:
		Header(string name, int max_height, int max_width) : Screen(name, max_height, max_width){
			
		}
		void AssembleOutputList() override {
			vector<string> header = {
			"  _____  _____  ____  _____  ______  _______     __",
			" / ____|/ ____|/ __ \\|  __ \\|  ____|/ ____\\ \\   / /",
			"| |    | (___ | |  | | |__) | |__  | (___  \\ \\_/ / ",
			"| |     \\___ \\| |  | |  ___/|  __|  \\___ \\  \\   /  ",
			"| |____ ____) | |__| | |    | |____ ____) |  | |   ",
			" \\_____|_____/ \\____/|_|    |______|_____/   |_|   ",
			" Hello, Welcome to CSOPESY CommandLine!            ",
			" Type 'exit' to quit, 'clear' to clear the screen  "
			};
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}
		}
};

class Input : public Screen{
	private:
		string input;
		bool is_command_done;
		vector<string> cli_list;
		int suggest;
		int suggest_index;
		vector<vector<string>> suggest_list;

	public:
		Input(int max_height, int max_width) : Screen("Input", max_height, max_width){
			this->input = "";
			this->is_command_done = false;
			this->suggest = -1;	
			this->suggest_index = 0;

			vector<string> suggestions_1 = {
				"",
				"screen -s",
				"screen -r",
				"clear",
				"exit",
			};

			vector<string> suggestions_2 = {
				"",
			};

			this->suggest_list.push_back(suggestions_1);
			this->suggest_list.push_back(suggestions_2);
		}

		//Getters and Setters

		void SetInput(string input){ this->input = input; }
		string GetInput(){ return this->input; }

		void SetCommandDone(bool is_command_done){ this->is_command_done = is_command_done; }
		bool GetCommandDone(){ return this->is_command_done; }

		void SetCliList(vector<string> cli_list){ this->cli_list = cli_list; }
		vector<string> GetCliList(){ return this->cli_list; }

		void SetSuggest(int suggest){ this->suggest = suggest; }
		int GetSuggest(){ return this->suggest; }

		void SetSuggestIndex(int suggest_index){ this->suggest_index = suggest_index; }
		int GetSuggestIndex(){ return this->suggest_index; }

		void SetSuggestList(vector<vector<string>> suggest_list){ this->suggest_list = suggest_list; }
		void SetSuggestList(int index, vector<string> suggest_list){ this->suggest_list[index] = suggest_list; }
		vector<vector<string>> GetSuggestList(){ return this->suggest_list; }

		// Other Methods

        void ClearCliList(){
            this->cli_list.clear();
        }

		void AddSuggestList(int index, string suggest){
			this->suggest_list[index].push_back(suggest);
			
		}

		void KeyInput(){
	
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
					if(key == '\t') {
						if(this->suggest == -1){
							if(this->input == ""){
								this->suggest = 0;
							}
							if(this->input == "screen -r "){
								this->suggest = 1;
							}
						}
						if(this->suggest == 0){
							this->suggest_index++;
							if(this->suggest_index >= this->suggest_list[this->suggest].size()){
								this->suggest_index = 0;
							}
							this->input = this->suggest_list[this->suggest][this->suggest_index];
						}
						if(this->suggest == 1){
							this->suggest_index++;
							if(this->suggest_index >= this->suggest_list[this->suggest].size()){
								this->suggest_index = 0;
							}
							this->input = "screen -r " + this->suggest_list[this->suggest][this->suggest_index];
						}
					}
					else{
						this->input += key;
						this->suggest = -1;
						this->suggest_index = 0;
					}
				}
			}
		}

		void Decoder(smatch* match, int* action){
			
			vector<regex> command_list = {
				//invalid						//-1
				(regex)"(clear)", 				//0
				(regex)"(exit)", 				//1
				(regex)"(screen -s (\\S+))",	//2
				(regex)"(screen -r (\\S+))",	//3
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

		void Push(string input){
			if(this->cli_list.size() == 0){
				this->cli_list.push_back(input);
			}
			else{
				if(input == "Invalid Command Line"){
					if(this->cli_list[this->cli_list.size()-1] == "Invalid Command Line"){
						this->cli_list[this->cli_list.size()-1] = input;
					}
					else{
						this->cli_list.push_back(input);
					}
				}
				else{
					if(this->cli_list.size() >= this->GetMaxHeight()){
						this->cli_list[this->cli_list.size()-1] = input;
					}
					else{
						this->cli_list.push_back(input);
					}
				}
			}
		}

		void AssembleOutputList() override {
            this->SetLine("Enter command: " + this->input, 0);
            this->SetLine("--------------------------------------------------------------", 1);
			for(int i = 0; i < this->cli_list.size(); i++){
				this->SetLine(this->cli_list[i], i + 2);
			}
		}
};