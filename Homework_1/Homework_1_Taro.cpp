#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;
#define ARR_SIZE 100

void Push(string input, vector<string>& list, int mode, int i){
	if(list.size() == 0){
		list.push_back(input);
	}
	else{
		if(mode == 0){
			if(list.size() > i){
				list[i] = input;
			}
			else{
				list.push_back(input);
			}
		}
		else{
			if("Invalid Command Line" == list[list.size()-1]){
			list[list.size()-1] = input;
			}
			else{
				list.push_back(input);
			}
		}
	}
}

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

		void ConsoleOut(vector<int> color_map){
			for(int i = 0; i < this->max_height; i++){
				this->MoveCursorTo({0, (short)i});
				this->ClearCurrentLine();
				this->SetColor(color_map[i]);
				cout << output_list[i] << "\n" << flush;
			}
		}
		
		COORD GetSize(){
			return {(short)max_width, (short)max_height};
		}

		void SetColor(int color) {
    		SetConsoleTextAttribute(this->h_console, color);
		}
};

void Input(string* input, bool* is_command_done){
	
    if(_kbhit()){
    	char key = _getch();
    	if (key == '\r' || key == '\n') {
    		*is_command_done = true;
    	}
    	else{
    		if (key == 8){
	 			if((*input).length() != 0){
	 				input->pop_back();
				}
			}
			else{
				*input += key;
			}
		}
	}
}

void Cli(string* input, int* action, bool* is_command_done){
	
	smatch match;
	
	vector<regex> command_list = {
		//invalid						//-1
		(regex)"(initialize)", 			//0
	    (regex)"(exit)", 				//1
	    (regex)"(screen -s (\\S+))",	//2
	    (regex)"(screen -ls)",			//3
	    (regex)"(scheduler-start)",		//4
	    (regex)"(scheduler-stop)",		//5
		(regex)"(report-util)",			//6
	    (regex)"(ping)",				//7
	    (regex)"(clear)"				//8
	};
	
	if(*is_command_done){

		bool is_valid = false;
		
		for(int i = 0; i < command_list.size(); i++){
			if(regex_match(*input, match, command_list[i])){
				is_valid = true;
				*action = i;
				i = command_list.size(); 
			}
		}
		if(!is_valid){
			*action = -1;
		}
        
        *input = "";
    }
}

void Interpreter(int action, vector<string>& cli_list, bool* is_command_quit, bool* is_command_clear){
	
	switch(action){
		case 0:
			Push("Initialize command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 1:
			Push("Goodbye", cli_list, 1, 0);
			*is_command_quit = true;
			break;
		case 2:
			Push("Screen -s command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 3:
			Push("Screen -ls command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 4:
			Push("Scheduler Start command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 5:
			Push("Scheduler Stop command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 6:
			Push("Report Utilization command recognized. Doing something.", cli_list, 1, 0);
			break;
		case 7:
			Push("Ping", cli_list, 1, 0);
			break;
		case 8:
			*is_command_clear = true;
			break;
		case -1:
			Push("Invalid Command Line", cli_list, 1, 0);
			break;
		default:
			break;
	}
}

int main() {

	Console console;

	vector<string> cli_list;

	int action;
	
	string input = "";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;
	
	int i = 0; 
	COORD console_size = console.GetSize();

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

	vector<int> color_map = { 
		7, 7, 7, 7, 7, 7, 10, 14,
		7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7,
		7, 7, 7, 7, 7, 7, 7, 7,
	};

	
	while(!is_command_quit){		
        Input(&input, &is_command_done);
		Cli(&input, &action, &is_command_done);
		if(is_command_done){
			Interpreter(action, cli_list, &is_command_quit, &is_command_clear);
			is_command_done = false;
			if(is_command_clear){
				console.ConsoleFlush(0);
				cli_list.clear();
				is_command_clear = false;
			}
		}
		console.ConsoleFill(0, header.size(), header);
		console.ConsoleFill(header.size(), "Enter a command: " + input);
		console.ConsoleFill(header.size() + 2, cli_list.size(), cli_list);
		console.ConsoleOut(color_map);
		Sleep(50);
		i++;
	}
	
    return 0;
}
