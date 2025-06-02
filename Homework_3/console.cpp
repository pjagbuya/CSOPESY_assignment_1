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

		Screen screen;
			
	public:	
		Console(): screen("CLI", 0, 0){

			SetConsoleSize();
            cout << "Console Size: " << this->max_width << " x " << this->max_height << endl;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}
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

		// Other Methods

		void SetConsoleSize(){
			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);

			this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
		}

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

		// Screen Commands

		void AssembleScreenOutputList(){
			
		}

		void ClearScreen(){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->output_list[i] = " ";
			}
		}

		void Run(){
			this->AssembleScreenOutputList();
			this->ConsoleOut();
		}
};