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
#define ARR_SIZE 200
#define SCREEN_CAPACITY 4

#include "header.cpp"
#include "input.cpp"

class Console {
	private:

		HANDLE h_console;
		int max_width;
		int max_height;

		array<string, ARR_SIZE> output_list;
		vector<vector<int>> color_map;
		vector<shared_ptr<Screen>> screen_list;
		array<int, SCREEN_CAPACITY> screen_index;

		smatch match;
		int action;
		bool is_command_quit;	

		int clock_cycle;
		
	public:	
		Console(){

			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);

		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}

			shared_ptr<Input> input = make_shared<Input>(15, this->max_width);
			this->screen_list.push_back(input);

			shared_ptr<Header> header = make_shared<Header>(this->max_height - 15, this->max_width);
			this->screen_list.push_back(header);

			this->screen_index = {-1, -1, -1, -1};
			this->action = -2;
			this->is_command_quit = false;
			this->clock_cycle = 0;
		} 

		//Getters and Setters

		void SetHandle(HANDLE h_console){ this->h_console = h_console; }
		HANDLE GetHandle(){ return this->h_console; }
		
		void SetMaxWidth(int max_width){ this->max_width = max_width; }
		int GetMaxWidth(){ return this->max_width; }

		void SetMaxHeight(int max_height){ this->max_height = max_height; }
		int GetMaxHeight(){ return this->max_height; }

		COORD GetSize(){ return {(short)max_width, (short)max_height}; }

		void SetOutputList(array<string, ARR_SIZE> output_list){ this->output_list = output_list; }
		array<string, ARR_SIZE> GetOutputList(){ return this->output_list; }

		void SetScreenList(vector<shared_ptr<Screen>> screen_list){ this->screen_list = screen_list; }
		vector<shared_ptr<Screen>> GetScreenList(){ return this->screen_list; }

		void SetScreenIndex(array<int, SCREEN_CAPACITY> screen_index){ this->screen_index = screen_index; }
		array<int, SCREEN_CAPACITY> GetScreenIndex(){ return this->screen_index; }
		void SetScreenIndex(int index, int value){ this->screen_index[index] = value; }

		void SetMatch(smatch match){ this->match = match; }
		smatch GetMatch(){ return this->match; }

		void SetAction(int action){ this->action = action; }
		int GetAction(){ return this->action; }

		void SetCommandQuit(bool is_command_quit){ this->is_command_quit = is_command_quit; }
		bool GetCommandQuit(){ return this->is_command_quit; }

		void SetClockCycle(int clock_cycle){ this->clock_cycle = clock_cycle; }
		int GetClockCycle(){ return this->clock_cycle; }
		
		// Cursor and Color Methods

		void MoveCursorTo(COORD coord) { SetConsoleCursorPosition(this->h_console, coord); }
		void SetColor(int color) { SetConsoleTextAttribute(this->h_console, color); }
		
		void ClearCurrentLine() {
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    DWORD count_written;
		    
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);
		    COORD line_start = { 0, csbi.dwCursorPosition.Y };
		    FillConsoleOutputCharacter(this->h_console, ' ', csbi.dwSize.X, line_start, &count_written);
		}

		// Console Commands
		
		void ConsoleFlush(int index){
			for(int i = index+1; i < ARR_SIZE; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->output_list[i] = " ";
			}
		}
	
		void ConsoleOut(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				cout << output_list[i] << "\n" << flush;
			}
		}

		void ConsoleOut(vector<int> color_map){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->SetColor();
				cout << output_list[i] << "\n" << flush;
			}
		}

		//Screen Commands

		bool AddScreenIndex(int index){
			for(int i = 0; i < SCREEN_CAPACITY; i++){
				if(this->screen_index[i] == -1){
					if(this->screen_index[i] != index){
						this->screen_index[i] = index;
					}
					else{
						this->screen_list[0]->Push("Screen " + this->screen_list[index]->GetName() + " already shown");
					}
					return true;
				}
			}
			return false;
		}

		bool ReplaceScreenIndex(int old_index, int new_index){
			for(int i = 0; i < SCREEN_CAPACITY; i++){
				if(this->screen_index[i] == old_index){
					if(old_index != new_index){
						this->screen_index[i] = new_index;
					}
					else{
						this->screen_list[0]->Push("Screen " + this->screen_list[new_index]->GetName() + " already shown");
					}
					return true;
				}
			}
			return false;
		}

		bool RemoveScreenIndex(int index){
			for(int i = 0; i < SCREEN_CAPACITY; i++){
				if(this->screen_index[i] == index){
					this->screen_index[i] = -1;
					return true;
				}
			}
			return false;
		}

		void AssignScreenIndex(int i, int index){
			this->screen_index[i] = index;
		}

		void CreateScreen(string name){
			for(auto& screen : this->screen_list){
				if(screen->GetName() == name){
					this->screen_list[0]->Push("Screen " + name + " already exists");
					return;
				}
			}
			shared_ptr<Screen> screen = make_shared<Screen>(name, this->max_height - 15, this->max_width);
			this->screen_list.push_back(screen);
			this->screen_list[this->screen_list.size()-1]->Initialize();
			this->screen_list[0]->Push("Screen " + name + " created");
			this->screen_list[0]->AddSuggestList(1, name);
		}

		void CallScreen(string name){
			bool is_valid = false;
			for(int i = 0; i < this->screen_list.size(); i++){
				if(this->screen_list[i]->GetName() == name){
					this->screen_index = i;
					this->screen_list[0]->Push("Screen " + name + " called");
					is_valid = true;
				}
			}
			if(!is_valid){
				this->screen_list[0]->Push("Screen " + name + " not found");
			}
		}

		void CallScreen(int index){
			if(index == -1){
				this->AddScreenIndex(index);
			}
			else{
				if(index < this->screen_list.size()){
					this->screen_index = index;
					this->screen_list[0]->Push("Screen " + this->screen_list[index]->GetName() + " called");
				}
			}
		}

		void AssembleScreenOutputList(){
			if(this->screen_index != -1){
                this->screen_list[this->screen_index].AssembleOutputList();
                this->screen_list[0]->AssembleOutputList();
                int i = 0;
                for(auto str : this->screen_list[this->screen_index].GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
                for(auto str : this->screen_list[0]->GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
            }
            else{
                this->screen_list[1]->AssembleOutputList();
                this->screen_list[0]->AssembleOutputList();
                int i = 0;
                for(auto str : this->screen_list[1]->GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
                for(auto str : this->screen_list[0]->GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
            }
		}

		void ClearScreen(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->output_list[i] = " ";
			}
		}

		// Actual CLI Methods

		void Interpreter(){

			switch(this->action){
				case 0:
                    this->screen_list[0]->ClearCliList();
					this->screen_list[0]->ClearOutputList();
					this->screen_list[0]->Push("Clearing screen");
					break;
				case 1:
					this->screen_list[0]->Push("Goodbye");
					this->is_command_quit = true;
					break;
				case 2:
					this->screen_list[0]->Push("Creating screen " + this->match[2].str());
					this->CreateScreen(this->match[2].str());
					break;
				case 3:
					this->screen_list[0]->Push("Calling screen " + this->match[2].str());
					this->ClearScreen();
					this->CallScreen(this->match[2].str());
					break;
				case -1:
					this->screen_list[0]->Push("Invalid Command Line");
					break;
				default:
					break;
			}
		}

		void ConsoleInput(){
			this->screen_list[0]->KeyInput();
			this->screen_list[0]->Decoder(&this->match, &this->action);
			if(this->screen_list[0]->GetCommandDone()){
                this->Interpreter();
				this->screen_list[0]->SetCommandDone(false);
				action = -2; 
			}
		}

		void Run(){
			this->ConsoleInput();
			this->AssembleScreenOutputList();
			this->ConsoleOut();
			this->clock_cycle++;
		}
};