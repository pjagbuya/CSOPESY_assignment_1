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
#define CLI_CAPACITY 11

#include "galaga.cpp"
#include "input.cpp"

void Debug(string message){ cout << message << "\n" << flush; Sleep(10); }

class Console {
	private:

		HANDLE h_console;
		int max_width;
		int max_height;

		array<string, ARR_SIZE> output_list;
		vector<vector<int>> color_map;
		Input input;
		Galaga galaga;
		int screen_index;
		int action;
		bool is_command_quit;	

		int clock_cycle;
		
	public:	
		Console(): input(CLI_CAPACITY, 0), galaga(0, 0){

			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);

		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}

			for(int i = 0; i < this->max_height; i++){
				this->color_map.push_back({});
				for(int j = 0; j < this->max_width; j++){
					this->color_map[i].push_back(7);
				}
			}

			this->input = input;
			this->input.SetMaxWidth(this->max_width);
			
			this->screen_index = 0;

			this->galaga.SetMaxHeight(this->max_height - 6);
			this->galaga.SetMaxWidth(this->max_width - 2);

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

		void SetColorMap(vector<vector<int>> color_map){ this->color_map = color_map; }
		vector<vector<int>> GetColorMap(){ return this->color_map; }

		void SetInput(Input input){ this->input = input; }
		Input GetInput(){ return this->input; }

		void SetGalaga(Galaga galaga){ this->galaga = galaga; }
		Galaga GetGalaga(){ return this->galaga; }

		void SetScreenIndex(int screen_index){ this->screen_index = screen_index; }
		int GetScreenIndex(){ return this->screen_index; }

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
				for(int j = 0; j < this->color_map[i].size(); j++){
					this->color_map[i][j] = 7;
				}
			}
		}

		void ConsoleOut(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				cout << output_list[i] << "\n" << flush;
			}
		}

		//Screen Commands
		string AssembleBorder(int count, int gap){
			int size = (this->max_width - gap - count * (gap + 2) - 2) / count;
			string line = "";
			line += "|";
			for(int i = 0; i < gap; i++){
				line += " ";
			}
			for(int i = 0; i < count; i++){
				line += "+";
				for(int j = 0; j < size; j++){
					line += "-";
				}
				line += "+";
				for(int j = 0; j < gap; j++){
					line += " ";
				}
			}
			line += "|";
			
			return line;
		}

		string AssembleLine(vector<string> str, int gap){
			int size = (this->max_width - gap - str.size() * (gap + 2) - 2) / str.size();
			string line = "";
			line += "|";
			for(int i = 0; i < gap; i++){
				line += " ";
			}
			for(int i = 0; i < str.size(); i++){
				line += "|";
				for(int j = 0 ; j < size; j++){
					if(j < str[i].length()){
						line += str[i][j];
					}
					else{
						line += " ";
					}
				}
				line += "|";
				for(int i = 0; i < gap; i++){
					line += " ";
				}
			}
			line += "|";
			
			return line;
		}

		void AssembleScreenColorList(){

		}

		void AssembleScreenOutputList(){
			this->galaga.AssembleOutputList();
			for(int i = 0; i < this->galaga.GetOutputList().size(); i++){
				this->output_list[i] = this->galaga.GetOutputList()[i];
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
					this->input.SetOutput("Paused");
					this->galaga.Pause();
					break;
				case 1:
					this->input.SetOutput("Resume");
					this->galaga.Resume();
					break;
				case 2:
					this->input.SetOutput(this->galaga.Upgrade());
					break;
				case 3:
					this->input.SetOutput(this->galaga.Heal());
					break;
				case -1:
					this->input.SetOutput("Invalid Command");
					break;
				default:
					break;
			}
		}

		void ConsoleInput(){
			this->input.KeyInput();
			this->input.Decoder(&this->action);
			if(this->input.GetCommandDone()){
                this->Interpreter();
				this->input.SetCommandDone(false);
				action = -2; 
			}
		}

		void Initialize(){
			
			this->input.Initialize();
			this->input.SetCommandDone(false);
			this->input.SetMoveDone(false);
			this->input.SetInput("");

			this->galaga.Initialize();

			Sleep(1000);
		}

		void Run(){
			this->ConsoleInput();
			this->galaga.Play(this->input.GetMove(), this->clock_cycle);
			this->AssembleScreenOutputList();
			this->ConsoleOut();
			this->clock_cycle++;
		}
};