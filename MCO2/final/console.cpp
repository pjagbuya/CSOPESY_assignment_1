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

#include "microprogramming.cpp"
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
		Microprogramming microprogramming;
		int action;
		bool is_command_quit;	

		int clock_cycle;

		smatch match;
		
	public:	
		Console(): input(CLI_CAPACITY), microprogramming(){

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

		void SetMicroprogramming(Microprogramming microprogramming){ this->microprogramming = microprogramming; }
		Microprogramming GetMicroprogramming(){ return this->microprogramming; }

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

		void AssembleScreenOutputList(){
			int buffer = this->microprogramming.GetOutputList().size();
			int max_size = this->output_list.size();

			// Copy microprogramming output
			for(int i = 0; i < this->microprogramming.GetOutputList().size() && i < max_size; i++){
				this->output_list[i] = this->microprogramming.GetOutputList()[i];
			}
			// Copy input output, but don't go out of bounds
			for(int i = 0; i < this->input.GetOutputList().size() && (i + buffer) < max_size; i++){
				this->output_list[i + buffer] = this->input.GetOutputList()[i];
			}
		}

		void ClearScreen(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->output_list[i] = " ";
			}
		}

		bool IsValidRegister(string input){
			if(input == "RAX" || input == "RBX" || input == "RCX" || input == "RDX" ||
			   input == "RDI" || input == "RSI" || input == "RBP" || input == "RSP") {
				return true;
			}
			return false;
		}

		bool IsValidHex(string input) {
			regex hexPattern("^0x[0-9A-Fa-f]{8}$");
			if (regex_match(input, hexPattern)){
				return true;
			}
			return false;
		}

		bool IsValidAddress(string input) {
			if (!IsValidHex(input)){
				return false;
			}
			uint32_t value = stoul(input.substr(2), nullptr, 16);
			return value <= 0x000000FF;
		}

		bool IsValidMMAccess(string input) {
			if(input.front() == '[' && input.back() == ']'){
				string inside = input.substr(1, input.size() - 2);
				if (IsValidAddress(inside)){
					return true;
				}
			}
			return false;
		}

		bool IsValidMMRegAccess(string input) {
			if(input.front() == '[' && input.back() == ']'){
				string inside = input.substr(1, input.size() - 2);
				if (IsValidRegister(inside)){
					return true;
				}
			}
			return false;
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
					this->input.Push("Pong");
					break;
				case 3:
					if(this->IsValidRegister(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							this->match[3].str() + "out, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidMMAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							this->match[3].str() + "out, MDRin",
							"IRAFout, MARin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[2];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							"MDRout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[3];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidMMRegAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[2];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							this->match[3].str() + "out, MDRin",
							reg + "out, MARin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMRegAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[3];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							"MDRout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else 
					if(this->IsValidRegister(this->match[2]) && this->IsValidHex(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRDFout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.WriteRegister("IRDF", this->match[3].str());
						this->microprogramming.SetMicrocode(microcode);
					}
					else{
						this->input.Push("Invalid MOV command");
					}
					this->action = -2;
					break;
				case 4:
					if(this->IsValidRegister(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							this->match[3].str() + "out, Yin",
							this->match[2].str() + "out, SELECT Y, ADD, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidMMAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, ADD, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[2];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[3].str() + "out, SELECT Y, ADD, Zin",
							"Zout, " + this->match[3].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[3];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidMMRegAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[2];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, ADD, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMRegAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[3];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[2].str() + "out, SELECT Y, ADD, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else{
						this->input.Push("Invalid ADD command");
					}
					this->action = -2;
					break;
				case 5:
					if(this->IsValidRegister(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							this->match[3].str() + "out, Yin",
							this->match[2].str() + "out, SELECT Y, SUB, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidMMAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, SUB, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[2];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[3].str() + "out, SELECT Y, SUB, Zin",
							"Zout, " + this->match[3].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[3];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidMMRegAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[2];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, SUB, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMRegAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[3];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[2].str() + "out, SELECT Y, SUB, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else{
						this->input.Push("Invalid SUB command");
					}
					this->action = -2;
					break;
				case 6:
					if(this->IsValidRegister(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							this->match[3].str() + "out, Yin",
							this->match[2].str() + "out, SELECT Y, XOR, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidMMAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, XOR, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[2];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						vector<string> microcode = {
							"IRAFout, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[3].str() + "out, SELECT Y, XOR, Zin",
							"Zout, " + this->match[3].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
						string address = this->match[3];
						address = address.substr(1, address.size() - 2);
						this->microprogramming.WriteRegister("IRAF", address);
					}
					else if(this->IsValidMMRegAccess(this->match[2]) && this->IsValidRegister(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[2];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							this->match[3].str() + "out, Yin",
							"MDRout, SELECT Y, XOR, Zin",
							"Zout, MDRin, WRITE, WMFC",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else if(this->IsValidRegister(this->match[2]) && this->IsValidMMRegAccess(this->match[3])){
						this->microprogramming.SetMicrocodeCounter(0);
						string reg = this->match[3];
						reg = reg.substr(1, reg.size() - 2);
						vector<string> microcode = {
							reg + "out, MARin, READ, WMFC",
							"MDRout, Yin",
							this->match[2].str() + "out, SELECT Y, XOR, Zin",
							"Zout, " + this->match[2].str() + "in",
							"END"
						};
						this->microprogramming.SetMicrocode(microcode);
					}
					else{
						this->input.Push("Invalid XOR command");
					}
					this->action = -2;
					break;
				case 7:
					if(this->IsValidAddress(this->match[2]) && this->IsValidHex(this->match[3])){
						this->microprogramming.MemoryWrite(this->match[3], this->match[2]);
					}
					this->action = -2;
					break;
				case -1:
					this->microprogramming.SetRCX("0xFFFFFFFF");
					this->input.Push("Invalid Command Line");
					break;
				default:
					break;
			}
		}

		void Initialize() {
			this->ClearScreen();
			this->microprogramming.Initialize();			
			this->input.Initialize();
			this->AssembleScreenOutputList();
		}

		void Run() {
			this->input.Run(&this->match, &this->action);
			this->Interpreter();
			this->microprogramming.Run(this->clock_cycle);
			this->AssembleScreenOutputList();
			this->ConsoleOut();
			this->clock_cycle++;
		}
};