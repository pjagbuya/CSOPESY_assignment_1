#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;
#define ARR_SIZE 100

class Console {
	private:
		int max_width;
		int max_height;
		array<string, ARR_SIZE> output_list;
		HANDLE h_console;
		
	public:	
		Console(){
			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(h_console, &csbi);
		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
		} 
		
		void MoveCursorTo(COORD coord) {
		    SetConsoleCursorPosition(this->h_console, coord);
		}
		
		void ClearCurrentLine() {
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    DWORD count_written;
		    
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);
		    COORD line_start = { 0, csbi.dwCursorPosition.Y };
		    FillConsoleOutputCharacter(this->h_console, ' ', csbi.dwSize.X, line_start, &count_written);
		}
		
		void ConsoleFill(int index, int count, vector<string> list){
			for(int i = 0; i < count; i++){
				this->output_list[index + i] = list[i];
			}
		}
		
		void ConsoleFill(int index, string input){
			this->output_list[index] = input;
		}
		
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
		
		COORD GetSize(){
			return {(short)max_width, (short)max_height};
		}
};