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
		
		COORD GetSize(){
			return {(short)max_width, (short)max_height};
		}
};

void Marquee(string marquee_text, int max_w, int max_h, COORD& marquee_pos, COORD& marquee_dir, int iter, int speed, vector<string>& marquee_list) {
        
    string output;
    //Marquee Printing
	for(int h = 0; h < max_h; h++){
		if(marquee_pos.Y == h){
    		for(int w = 0; w < max_w; w++){
    			if(marquee_pos.X == w){
    				output += marquee_text;
				}
				else{
					output += " ";
				}
			}
		}
		else{
			output = " ";
		}
		Push(output, marquee_list, 0, h);
	}
	
	if(iter%speed == 0){
		if(marquee_dir.X == 1 && marquee_pos.X >= max_w-1-marquee_text.length()){
			marquee_dir.X = -1;
		}
		if(marquee_dir.X == -1 && marquee_pos.X <= 0){
			marquee_dir.X = 1;
		}
		if(marquee_dir.Y == 1 && marquee_pos.Y >= max_h-1){
			marquee_dir.Y = -1;
		}
		if(marquee_dir.Y == -1 && marquee_pos.Y <= 0){
			marquee_dir.Y = 1;
		}
	
		marquee_pos.X += marquee_dir.X;
		marquee_pos.Y += marquee_dir.Y;
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
		regex(R"(and (\S+) (\S+))"),			// 0
		regex(R"(ping)"),						// 1
		regex(R"(jarvis)"),						// 2
		regex(R"(clive)"),						// 3
		regex(R"(kenneth)"),					// 4
		regex(R"(pranjeet (\d+))"),				// 5
		regex(R"(rinaldo (\d+) (\S+))"),		// 6
		regex(R"(text ([^>\n]+))"),				// 7
		regex(R"(speed (\d+))"),				// 8
		regex(R"(clear)"),						// 9
		regex(R"(quit)")						// 10
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
		case 0: // and
			Push(match[1].str() + " and " + match[2].str(), cli_list, 1, 0);
			break;
		case 1: // ping
			Push("pong", cli_list, 1, 0);
			break;
		case 2: // jarvis
			Push("Hit the Griddy", cli_list, 1, 0);
			break;
		case 3: // clive
			Push("Clivetto Jarelini Angustino, Bombardiro Della Pesto Nuclearo", cli_list, 1, 0);
			break;
		case 4: // kenneth
			Push("Skill Issue", cli_list, 1, 0);
			break;
		case 5: // pranjeet
			for (int i = 0; i < stoi(match[1]); i++) {
				str += "Uii" + string(i, 'i') + "ai ";
			}
			Push(str, cli_list, 1, 0);
			break;
		case 6: // rinaldo
			str = "I am Rinaldo, I am hungry! ";
			for (int i = 0; i < stoi(match[1]); i++) {
				str += "Nom! ";
			}
			str += match[2].str() + "!";
			Push(str, cli_list, 1, 0);
			break;
		case 7: // text
			*marquee_text = match[1].str();
			Push("Changed text to " + *marquee_text, cli_list, 1, 0);
			break;
		case 8: // speed
			*speed = stoi(match[1]);
			Push("Changed speed to " + to_string(*speed), cli_list, 1, 0);
			break;
		case 9: // clear
			*is_command_clear = true;
			break;
		case 10: // quit
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

	Console console;
    
	int speed = 1;
	string marquee_text = "Hello World in Marquee!";
	COORD marquee_pos = {0,0}; //Position
	COORD marquee_dir = {1,1}; //Direction
	
	vector<string> marquee_list;
	vector<string> cli_list;

	int action;
	
	string input = "";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;
	smatch match;
	
	int i = 0; 
	COORD console_size = console.GetSize();
	
	while(!is_command_quit){
		Marquee(marquee_text, console_size.X, console_size.Y - 10, ref(marquee_pos), ref(marquee_dir), i, speed, marquee_list);
		Input(&input, &is_command_done);
		Cli(&input, &action, &is_command_done, match);
		if(is_command_done){
			Interpreter(action, cli_list, &is_command_quit, &is_command_clear, &speed, &marquee_text, match);
			is_command_done = false;
			if(is_command_clear){
				console.ConsoleFlush(marquee_list.size());
				cli_list.clear();
				is_command_clear = false;
			}
		}
		console.ConsoleFill(0, marquee_list.size(), marquee_list);
		console.ConsoleFill(marquee_list.size(), "Enter input: " + input);
		console.ConsoleFill(marquee_list.size() + 1, cli_list.size(), cli_list);
		console.ConsoleOut();
		Sleep(50);
		i++;
	}
	
    return 0;
}
