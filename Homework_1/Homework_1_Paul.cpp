#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>

using namespace std;

#define ARR_SIZE 200

void Push(string input, vector<string>& list, int mode, int i) {
	if (list.size() == 0) {
		list.push_back(input);
	}
	else {
		if (mode == 0) {
			if (list.size() > i) {
				list[i] = input;
			}
			else {
				list.push_back(input);
			}
		}
		else {
			if ("Invalid Command Line" == list[list.size() - 1]) {
				list[list.size() - 1] = input;
			}
			else {
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
	Console() {
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

	void ConsoleFill(int index, int count, vector<string> list) {
		for (int i = 0; i < count; i++) {
			this->output_list[index + i] = list[i];
		}
	}

	void ConsoleFill(int index, string input) {
		this->output_list[index] = input;
	}

	void ConsoleFlush(int index) {
		for (int i = index + 1; i < ARR_SIZE; i++) {
			this->MoveCursorTo({ 0, (short)i });
			this->ClearCurrentLine();
			this->output_list[i] = " ";
		}
	}

	void ConsoleOut() {
		for (int i = 0; i < this->max_height; i++) {
			this->MoveCursorTo({ 0, (short)i });
			this->ClearCurrentLine();
			cout << output_list[i] << "\n" << flush;
		}
	}

	COORD GetSize() {
		return { (short)max_width, (short)max_height };
	}
};

void Input(string* input, bool* is_command_done) {

	if (_kbhit()) {
		char key = _getch();
		if (key == '\r' || key == '\n') {
			*is_command_done = true;
		}
		else {
			if (key == 8) {
				if ((*input).length() != 0) {
					input->pop_back();
				}
			}
			else {
				*input += key;
			}
		}
	}
}


void CLI(string* input, vector<string>& cli_list, bool* is_command_quit, bool* is_command_done, bool* is_command_clear) {
	static const std::vector<std::string> fixed_commands = {
	"initialize",
	"screen",
	"scheduler-test",
	"scheduler-stop",
	"report-util",
	"clear",
	"exit"
	};

	regex and_regex(R"(and (\S+) (\S+))");
	regex ping_regex(R"(ping)");
	regex clear_regex(R"(clear)");
	regex quit_regex(R"(exit)");
	smatch match;


	bool fixed_command_matched = false;
	bool avail_clear_quit = false;
	string matched_command_str;
	if (*is_command_done) {
		for (const auto& cmd : fixed_commands) {
			if (*input == cmd) {
				fixed_command_matched = true;
				matched_command_str = cmd; // Store the matched command string
				Push(cmd + " command is recognized. Doing something.", cli_list, 0, cli_list.size());

				if (!(strcmp(cmd.c_str()	, "exit") && strcmp(cmd.c_str(), "clear"))) {
					avail_clear_quit = true;
				}
				break; // Found a match, no need to check the rest of the fixed commands
			}
		}
		
		
		if (fixed_command_matched && !avail_clear_quit){
		}
		else if (regex_match(*input, match, and_regex))
		{
			Push((match[1].str() + " and " + match[2].str()), cli_list, 1, 0);
		}
		else if (regex_match(*input, ping_regex))
		{
			Push("pong", cli_list, 1, 0);
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

	vector<string> cli_list;
	vector<string> ascii_art = {
		" _____  _____  ___________ _____ _______   __",
		"/  __ \\/  ___||  _  | ___ \\  ___/  ___\\ \\ / /",
		"| /  \\/\\ `--. | | | | |_/ / |__ \\ `--. \\ V / ",
		"| |     `--. \\| | | |  __/|  __| `--. \\ \\ /  ",
		"| \\__/\\/\\__/ /\\ \\_/ / |   | |___/\\__/ / | |  ",
		" \\____/\\____/  \\___/\\_|   \\____/\\____/  \\_/  "
	};

	


	string input = "";
	string hello1 = "\033[32mHello welcome to CSOPESY command line.\033[0m\n";
	string hello2 = "\033[33mType 'exit' to quit, 'clear' to clear the screen\033[0m\n";
	string barline = "\033[33m---------------------------------------------------\033[0m\n";
	bool is_command_quit = false;
	bool is_command_done = false;
	bool is_command_clear = false;

	int i = 0;
	int cli_size=0;
	COORD console_size = console.GetSize();

	while (!is_command_quit) {
		Input(&input, &is_command_done);
		CLI(&input, cli_list, &is_command_quit, &is_command_done, &is_command_clear);

		// This ensures command clear is resetted, but reaffirms the flushing of outputs
		if (is_command_clear) {
			console.ConsoleFlush(0);
			is_command_clear = false;
		}	
		console.ConsoleFill(0, ascii_art.size(), ascii_art);
		console.ConsoleFill(6, hello1);
		console.ConsoleFill(7, hello2);
		
		console.ConsoleFill(8, "Enter input: " + input);
		console.ConsoleFill(9, barline);
		console.ConsoleFill(10, cli_list.size(), cli_list);
		console.ConsoleOut();
		Sleep(100);
		i++;
	}

	return 0;
}
