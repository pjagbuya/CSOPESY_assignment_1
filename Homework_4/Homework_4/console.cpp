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
#define SCREEN_CAPACITY 4
#define CLI_CAPACITY 11

#include "scheduler.cpp"
#include "header.cpp"
#include "input.cpp"

void Debug(string message){ cout << message << "\n" << flush; Sleep(10); }

class Console {
	private:

		HANDLE h_console;
		int max_width;
		int max_height;

		array<string, ARR_SIZE> output_list;
		Scheduler scheduler;
		Input input;
		Header header;

		smatch match;
		int action;
		bool is_command_quit;	
		int clock_cycle;
		
	public:	
		Console(): scheduler(), input(0), header(0){

			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);

		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}

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

		// Console Methods
		
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

		void AssembleScreenOutputList(){

		}

		void ClearScreen(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->output_list[i] = " ";
			}
		}

		// Screen Methods

		void CreateScreen(string name){
			for(auto process : this->scheduler.GetReadyQueues()[0].GetProcesses()){
				if(process.GetName() == name){
					this->input.Push("Screen " + name + " already exists");
					return;
				}
			}
			Process new_process(name, 0, 0, 0, 0, "Screen " + name + " says Hello World!");
			this->scheduler.GetReadyQueues()[0].AddProcess(new_process);
		}

		void CallScreen(string name){
			bool is_valid = false;
			for(int i = 0; i < this->scheduler.GetReadyQueues()[0].GetProcesses().size(); i++){
				if(this->scheduler.GetReadyQueues()[0].GetProcesses()[i].GetName() == name){
					is_valid = true;
					this->input.Push("Calling screen " + name);
					this->scheduler.GetReadyQueues()[0].GetProcesses()[i].Run();
					break;
				}
			}
		}

		void ListScreen(){

		}

		// Actual CLI Methods

		void Interpreter(){

			switch(this->action){
				case 0:
                    this->input.ClearCliList();
					this->input.ClearOutputList();
					this->input.Push("Clearing screen");
					break;
				case 1:
					this->input.Push("Goodbye");
					this->is_command_quit = true;
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
				case 4:
					this->input.Push("Listing screen " + this->match[2].str());
					this->ClearScreen();
					this->ListScreen();
					break;
				case 5:
					this->input.Push("Pong");
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

		void Initialize(){
			this->input.Initialize();
			this->input.SetCommandDone(false);
			this->input.SetInput("");
			Sleep(1000);
		}

		void Run(){
			this->ConsoleInput();
			this->AssembleScreenOutputList();
			this->ConsoleOut();
			this->clock_cycle++;
		}
};