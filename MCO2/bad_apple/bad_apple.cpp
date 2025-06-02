#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <cstdlib>
using namespace std;
#define ARR_SIZE 200

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
		
		COORD GetSize(){
			return {(short)max_width, (short)max_height};
		}
};

void BadApple(int i, vector<string>& badapple_list) {
    const int FRAME_HEIGHT = 37;
    const int TOTAL_FRAMES = 6572;

    int target_frame = i % TOTAL_FRAMES;

    // Construct the filename: outXXXX.jpg.txt
    stringstream filename;
    filename << "frames-ascii/out" << setfill('0') << setw(4) << (target_frame + 1) << ".jpg.txt";

    ifstream file(filename.str());
    if (!file.is_open()) {
        cerr << "Failed to open frame file: " << filename.str() << endl;
        return;
    }

    string line;
    int j = 0;

    while (getline(file, line) && j < FRAME_HEIGHT) {
        Push(line, badapple_list, 0, j);
        j++;
    }
}

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

void Cli(string* input, int* action, bool* is_command_done, smatch& match){

	vector<regex> command_list = {
		//invalid								//-1
		regex(R"(ping)"),						// 0
		regex(R"(speed (\d+))"),				// 1
		regex(R"(clear)"),						// 2
		regex(R"(quit)")						// 3
	};

	bool is_valid = false;

	if(*is_command_done){
		
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

void Interpreter(int action, vector<string>& cli_list, bool* is_command_quit, bool* is_command_clear, int* speed, string* marquee_text, smatch match) {

	string str = "";

	switch(action){
		case 0: // ping
			Push("pong", cli_list, 1, 0);
			break;
		case 1: // speed
			*speed = stoi(match[1]);
			Push("Changed speed to " + to_string(*speed), cli_list, 1, 0);
			break;
		case 2: // clear
			*is_command_clear = true;
			break;
		case 3: // quit
			*is_command_quit = true;
			Push("Goodbye", cli_list, 1, 0);
			break;
		case -1: // invalid
			Push("Invalid Command Line", cli_list, 1, 0);
			break;
		default:
			break;
	}
}


int main() {

	HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);  // Maximize the console

	Console console;
    
	int speed;

    cin >> speed;

	string marquee_text = "Hello World in Marquee!";
	COORD marquee_pos = {0,0}; //Position
	COORD marquee_dir = {1,1}; //Direction
	
	vector<string> badapple_list;
	vector<string> cli_list;

	int action;
	
	string input = "";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;
	smatch match;
	
	int i = 0; 
	COORD console_size = console.GetSize();

	PlaySoundA("bad_apple.wav", NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
	
	while(!is_command_quit){
		BadApple(i, badapple_list);
		Input(&input, &is_command_done);
		Cli(&input, &action, &is_command_done, match);
		if(is_command_done){
			Interpreter(action, cli_list, &is_command_quit, &is_command_clear, &speed, &marquee_text, match);
			is_command_done = false;
			if(is_command_clear){
				console.ConsoleFlush(badapple_list.size());
				cli_list.clear();
				is_command_clear = false;
			}
		}
		console.ConsoleFill(0, badapple_list.size(), badapple_list);
		console.ConsoleFill(badapple_list.size(), "Enter input: " + input);
		console.ConsoleFill(badapple_list.size() + 1, cli_list.size(), cli_list);
		console.ConsoleOut();
		Sleep(speed);
		i++;
	}
	
    return 0;
}
