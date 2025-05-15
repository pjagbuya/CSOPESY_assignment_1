#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "console.cpp"
#include "os.cpp"

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
        *is_command_done = false;
    }
}

int main() {

	Console console;

	vector<string> cli_list;
	
	string input = "";
	bool is_command_done = false;
	bool is_command_quit = false;
	bool is_command_clear = false;
	
	int action;
	
	int cpu_cycles = 0; 
	COORD console_size = console.GetSize();
	
	while(!is_command_quit){
		Input(&input, &is_command_done);
		Cli(&input, &action, &is_command_done);
		
		console.ConsoleFill(0, "Enter input: " + input);
		console.ConsoleOut();
		
		Sleep(100);
		cpu_cycles++;
	}
	
    return 0;
}
