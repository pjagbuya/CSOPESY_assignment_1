#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
using namespace std;
#define ARR_SIZE 100

#include "screen.cpp"

class Console {
	private:

		HANDLE h_console;
		int max_width;
		int max_height;

		array<string, ARR_SIZE> output_list;

		vector<Screen> screen_list;
		int screen_index;
		
        Header header;
		Input input;
		smatch match;
		int action;
		bool is_command_quit;
		bool is_command_clear;		
		
	public:	
		Console() : header("header", 0, 0), input(0, 0){

			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);

		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
            cout << "Console Size: " << this->max_width << " x " << this->max_height << endl;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}

			this->header = header;
			this->header.SetMaxHeight(this->max_height - 15);
			this->header.SetMaxWidth(this->max_width);
			this->header.Initialize();
			this->screen_index = -1;

			this->input = input;
			this->input.SetMaxHeight(15);
			this->input.SetMaxWidth(this->max_width);
			this->input.Initialize();
			this->action = -2;
			this->is_command_quit = false;
			this->is_command_clear = false;
		} 

		//Getters and Setters

		void SetHandle(HANDLE h_console){ this->h_console = h_console; }
		HANDLE GetHandle(){ return this->h_console; }
		
		void SetMaxWidth(int max_width){ this->max_width = max_width; }
		int GetMaxWidth(){ return this->max_width; }

		void SetMaxHeight(int max_height){ this->max_height = max_height; }
		int GetMaxHeight(){ return this->max_height; }

		void SetOutputList(array<string, ARR_SIZE> output_list){ this->output_list = output_list; }
		array<string, ARR_SIZE> GetOutputList(){ return this->output_list; }

		void SetScreenList(vector<Screen> screen_list){ this->screen_list = screen_list; }
		vector<Screen> GetScreenList(){ return this->screen_list; }

		void SetScreenIndex(int screen_index){ this->screen_index = screen_index; }
		int GetScreenIndex(){ return this->screen_index; }

		void SetInput(Input input){ this->input = input; }
		Input GetInput(){ return this->input; }

		void SetMatch(smatch match){ this->match = match; }
		smatch GetMatch(){ return this->match; }

		void SetAction(int action){ this->action = action; }
		int GetAction(){ return this->action; }

		void SetCommandQuit(bool is_command_quit){ this->is_command_quit = is_command_quit; }
		bool GetCommandQuit(){ return this->is_command_quit; }

		void SetCommandClear(bool is_command_clear){ this->is_command_clear = is_command_clear; }
		bool GetCommandClear(){ return this->is_command_clear; }

		// Other Methods

		void MoveCursorTo(COORD coord) { SetConsoleCursorPosition(this->h_console, coord); }
		COORD GetSize(){ return {(short)max_width, (short)max_height}; }
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
				this->SetColor(color_map[i]);
				cout << output_list[i] << "\n" << flush;
			}
		}

		//Screen Commands

		void CreateScreen(string name){
			for(auto screen : this->screen_list){
				if(screen.GetName() == name){
					this->input.Push("Screen " + name + " already exists");
					return;
				}
			}
			Screen screen(name, this->max_height - 15, this->max_width);
			this->screen_list.push_back(screen);
			this->screen_list[this->screen_list.size()-1].Initialize();
			this->input.Push("Screen " + name + " created");
			this->input.AddSuggestList(1, name);
		}

		void CallScreen(string name){
			bool is_valid = false;
			for(int i = 0; i < this->screen_list.size(); i++){
				if(this->screen_list[i].GetName() == name){
					this->screen_index = i;
					this->input.Push("Screen " + name + " called");
					is_valid = true;
				}
			}
			if(!is_valid){
				this->input.Push("Screen " + name + " not found");
			}
		}

		void CallScreen(int index){
			if(index == -1){
				this->screen_index = -1;
			}
			else{
				if(index < this->screen_list.size()){
					this->screen_index = index;
					this->input.Push("Screen " + this->screen_list[index].GetName() + " called");
				}
			}
		}

		void AssembleScreenOutputList(){
			if(this->screen_index != -1){
                this->screen_list[this->screen_index].AssembleOutputList();
                this->input.AssembleOutputList();
                int i = 0;
                for(auto str : this->screen_list[this->screen_index].GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
                for(auto str : this->input.GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
            }
            else{
                this->header.AssembleOutputList();
                this->input.AssembleOutputList();
                int i = 0;
                for(auto str : this->header.GetOutputList()){
                    this->output_list[i] = str;
                    i++;
                }
                for(auto str : this->input.GetOutputList()){
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

		void Interpreter(){

			switch(this->action){
				case 0:
					this->is_command_clear = true;
                    this->input.ClearCliList();
					this->input.ClearOutputList();
					this->input.Push("Clearing screen");
					break;
				case 1:
					if(this->screen_index != -1){
						this->input.Push("Exiting screen");
						this->CallScreen(-1);
					}
					else{
						this->input.Push("Goodbye");
						this->is_command_quit = true;
					}
					break;
				case 2:
					this->input.Push("Creating screen " + this->match[2].str());
					this->CreateScreen(this->match[2].str());
					break;
				case 3:
					this->input.Push("Calling screen " + this->match[2].str());
					this->ClearScreen();
					this->CallScreen(this->match[2].str());
					break;
				case -1:
					this->input.Push("Invalid Command Line");
					break;
				default:
					break;
			}
		}

		void ConsoleInput(){
			this->input.KeyInput();
			this->input.Decoder(&this->match, &this->action);
			if(this->input.GetCommandDone()){
                this->Interpreter();
				this->input.SetCommandDone(false);
				action = -2; 
			}
		}
};