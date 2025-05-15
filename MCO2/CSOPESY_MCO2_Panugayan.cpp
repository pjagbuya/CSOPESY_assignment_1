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

void CLI(int* speed, string* marquee_text, string* input, vector<string>& cli_list, bool* is_command_quit, bool* is_command_done, bool* is_command_clear) {
	
	regex and_regex(R"(and (\S+) (\S+))");
    regex ping_regex(R"(ping)");
    regex jarvis_regex(R"(jarvis)");
    regex clive_regex(R"(clive)");
    regex kenneth_regex(R"(kenneth)");
    regex pranjeet_regex(R"(pranjeet (\d+))");
    regex rinaldo_regex(R"(rinaldo (\d+) (\S+))");
	regex text_regex(R"(text ([^>\n]+))");
    regex speed_regex(R"(speed (\d+))");
    regex clear_regex(R"(clear)");
    regex quit_regex(R"(quit)");
	smatch match;

	if(*is_command_done){
        if (regex_match(*input, match, and_regex)) 
		{
            Push((match[1].str() + " and " + match[2].str()), cli_list, 1, 0);
        } 
		else if (regex_match(*input, ping_regex)) 
		{
            Push("pong", cli_list, 1, 0);
        } 
		else if (regex_match(*input, jarvis_regex)) 
		{
            Push("Hit the Griddy", cli_list, 1, 0);
        } 
        else if (regex_match(*input, clive_regex)) 
		{
            Push("Clivetto Jarelini Angustino, Bombardiro Della Pesto Nuclearo", cli_list, 1, 0);
        } 
        else if (regex_match(*input, match, kenneth_regex)) 
		{
            Push("Skill Issue", cli_list, 1, 0);
        } 
        else if (regex_match(*input, match, pranjeet_regex)) 
		{
			string str = "";
            for(int i = 0; i < stoi(match[1]); i++){
            	str += "Uii";
            	for(int j = 0; j < i; j++){
            		str += "i";
				}
            	str += "ai ";
			}
			Push(str, cli_list, 1, 0);
        } 
        else if (regex_match(*input, match, rinaldo_regex)) 
		{
			string str = "I am Rinaldo, I am hungry! ";
            for(int i = 0; i < stoi(match[1]); i++){
            	str += "Nom! ";
            }
            str += match[2].str() + "!";
            Push(str, cli_list, 1, 0);
        } 
        else if (regex_match(*input, match, text_regex)) 
		{
            *marquee_text = match[1].str();
            
            Push("Changed text to " + *marquee_text, cli_list, 1, 0);
        } 
		else if (regex_match(*input, match, speed_regex)) 
		{
            *speed = stoi(match[1]);
            
            Push("Changed speed to " + to_string(*speed), cli_list, 1, 0);
        } 
        else if (regex_match(*input, clear_regex)) 
		{
			*is_command_clear = true; 
			
			cli_list.clear();
            system("cls");
        } 
        else if (regex_match(*input, quit_regex)) 
		{
			*is_command_quit = true; 	
				
            Push("Goodbye", cli_list, 1, 0);
        } 
		else {
            Push("Invalid Command Line", cli_list, 1, 0);
        }
        *input = "";
        *is_command_done = false;
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
	
	string input = "";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;
	
	int i = 0; 
	COORD console_size = console.GetSize();
	
	while(!is_command_quit){
		Marquee(marquee_text, console_size.X, console_size.Y - 10, ref(marquee_pos), ref(marquee_dir), i, speed, marquee_list);
		Input(&input, &is_command_done);
		CLI(&speed, &marquee_text, &input, cli_list, &is_command_quit, &is_command_done, &is_command_clear);
		console.ConsoleFill(0, marquee_list.size(), marquee_list);
		console.ConsoleFill(marquee_list.size(), "Enter input: " + input);
		console.ConsoleFill(marquee_list.size() + 1, cli_list.size(), cli_list);
		if(is_command_clear){
			console.ConsoleFlush(marquee_list.size());
			is_command_clear = false;
		}
		console.ConsoleOut();
		Sleep(50);
		i++;
	}
	
    return 0;
}
