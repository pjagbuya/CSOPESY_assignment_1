#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
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
			for(int i = 0; i < max_height; i++){
				this->output_list.push_back(" ");
			}
		}
		
		void SetName(string name){
			this->name = name;
		}
		
		string GetName(){
			return this->name;
		}
		
		time_t GetTime(){
			return this->time_created;
		}

		vector<string> GetOutputList(){
			return this->output_list;
		}
		
		void SetLine(string str, int i){
			this->output_list[i] = str;
		}

};

class Header : public Screen {
	public:
		Header(string name, int max_height, int max_width) : Screen(name, max_height, max_width){
			this->AssembleOutputList();
		}
		void AssembleOutputList(){
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

class Process : public Screen {
	public:
		Process(string name, int max_height, int max_width) : Screen(name, max_height, max_width){
			this->AssembleOutputList();
		}
		void AssembleOutputList(){
			time_t time = this->GetTime();
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
};

class Input : public Screen{
	public:
		Input(int max_width) : Screen("Input", 10, max_width){
			
		}
};

class Console {
	private:
		int max_width;
		int max_height;
		array<string, ARR_SIZE> output_list;
		HANDLE h_console;
		Input input;
		vector<Screen> screen_list;
		int screen_index;
		
	public:	
		Console(){
			this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    GetConsoleScreenBufferInfo(h_console, &csbi);
		    this->max_width = csbi.srWindow.Right;
		    this->max_height = csbi.srWindow.Bottom;
			this->screen_index = 0;
			for(int i = 0; i < ARR_SIZE; i++){
				this->output_list[i] = " ";
			}
			Header header("header", this->max_height, this->max_width);
			this->screen_list.push_back(header);
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

		int GetScreenIndex(){
			return this->screen_index;
		}

		void SetScreenIndex(int index){
			this->screen_index = index;
		}

		void SetScreenIndex(string name){
			for(int i = 0; i < this->screen_list.size(); i++){
				if(this->screen_list[i].GetName() == name){
					this->screen_index = i;
					break;
				}
			}
		}
		
		void SetColor(int color) {
    		SetConsoleTextAttribute(this->h_console, color);
		}

		void CreateScreen(string name){
			Screen screen(name, this->max_height, this->max_width);
			this->screen_list.push_back(screen);
		}

		void CallScreen(string name){
			for(int i = 0; i < this->screen_list.size(); i++){
				if(this->screen_list[i].GetName() == name){
					vector<string> output = this->screen_list[i].GetOutputList();
					for(int i = 0; i < output.size(); i++){
						this->output_list[i] = output[i];
						this->screen_index = i;
					}
					break;
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

void Cli(string* input, int* action, bool* is_command_done, smatch* match){
	
	vector<regex> command_list = {
		//invalid						//-1
		(regex)"(clear)", 				//0
	    (regex)"(exit)", 				//1
	    (regex)"(screen -s (\\S+))",	//2
	    (regex)"(screen -r (\\S+))",	//3
	};

	bool is_valid = false;
	
	if(*is_command_done){
		
		for(int i = 0; i < command_list.size(); i++){
			if(regex_match(*input, *match, command_list[i])){
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

void Interpreter(int action, vector<string>& cli_list, smatch match, bool* is_command_quit, bool* is_command_clear, Console* console){
	
	switch(action){
		case 0:
			*is_command_clear = true;
			break;
		case 1:
			if(console->GetScreenIndex() != 0){
				Push("Exiting screen", cli_list, 1, 0);
				console->CallScreen("Header");
			}
			else{
				Push("Goodbye", cli_list, 1, 0);
				*is_command_quit = true;
			}
			break;
		case 2:
			Push("Creating screen " + match[0].str(), cli_list, 1, 0);
			console->CreateScreen(match[1].str());
			break;
		case 3:
			Push("Calling screen " + match[0].str(), cli_list, 1, 0);
			console->ClearScreen();
			console->CallScreen(match[1].str());
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
	
	smatch match;

	string input = "";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;
	
	int i = 0; 
	COORD console_size = console.GetSize();

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
		Cli(&input, &action, &is_command_done, &match);
		if(is_command_done){
			Interpreter(action, cli_list, match, &is_command_quit, &is_command_clear, &console);
			is_command_done = false;
			if(is_command_clear){
				console.ConsoleFlush(0);
				cli_list.clear();
				is_command_clear = false;
			}
		}
		console.ConsoleFill(0, console size(), header);
		console.ConsoleFill(header.size(), "Enter a command: " + input);
		console.ConsoleFill(header.size() + 2, cli_list.size(), cli_list);
		console.ConsoleOut(color_map);
		Sleep(50);
		i++;
	}
	
    return 0;
}
