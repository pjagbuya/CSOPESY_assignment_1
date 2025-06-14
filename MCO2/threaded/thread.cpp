#include <iostream>
#include <thread>
#include <windows.h>
#include <mutex>
#include <regex>
#include <atomic>
#include <string>
#include <shared_mutex>

class AtomicString {
private:
    std::string value;
    mutable std::mutex mtx;

public:
    void set(const std::string& new_value) {
        std::lock_guard<std::mutex> lock(mtx);
        value = new_value;
    }

    std::string get() const {
        std::lock_guard<std::mutex> lock(mtx);
        return value;
    }
};

std::mutex io_mutex;

void moveCursorTo(COORD coord) {
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
	
    SetConsoleCursorPosition(h_console, coord);
}

COORD getCursorPos() {
	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(h_console, &csbi);
    return csbi.dwCursorPosition;
}

void clearCurrentLine() {
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count_written;
    
    GetConsoleScreenBufferInfo(h_console, &csbi);
    COORD line_start = { 0, csbi.dwCursorPosition.Y };
    FillConsoleOutputCharacter(h_console, ' ', csbi.dwSize.X, line_start, &count_written);
}

void clearLines(int start_line, int end_line) {
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count_written;

    GetConsoleScreenBufferInfo(h_console, &csbi);
    int width = csbi.dwSize.X;  // Get the console width
    for (short y = start_line; y <= end_line; y++) {
        COORD line_start = { 0, y };
        FillConsoleOutputCharacter(h_console, ' ', width, line_start, &count_written);
    }
}


void marquee(std::atomic<bool>& is_quit, std::atomic<int>& speed, AtomicString& marquee_text, int max_w, int max_h) {
	
	COORD marquee_pos = {0,0}; //Position
	COORD marquee_dir = {1,1}; //Direction
	
    while(!is_quit) {
        {
        	//Move cursor 
            std::lock_guard<std::mutex> lock(io_mutex);
            COORD saved_pos = getCursorPos();
            moveCursorTo({0,0});
            clearLines(0, max_h);
            
            //Marquee Printing
        	for(int h = 0; h < max_h; h++){
        		if(marquee_pos.Y == h){
	        		for(int w = 0; w < max_w; w++){
	        			if(marquee_pos.X == w){
	        				std::cout << marquee_text.get() << std::flush;
						}
						else{
							std::cout << " " << std::flush;
						}
					}
				}
				else{
					std::cout << "\n" << std::flush;
				}
			}
            
            moveCursorTo(saved_pos);
            
        	// Marquee Clivetto Jarelini Angustino Logico
        	if(marquee_dir.X == 1 && marquee_pos.X >= max_w-1){
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
        Sleep(speed);
    }
}

void console(std::atomic<bool>& is_quit, std::atomic<int>& speed, AtomicString& marquee_text, short y) {
	
	std::string input;
    std::regex ping_regex(R"(ping)");
	std::regex text_regex(R"(text ([^>\n]+))");
    std::regex speed_regex(R"(speed (\d+))");
    std::regex clear_regex(R"(clear)");
    std::regex quit_regex(R"(quit)");
	std::smatch match;
	
	short console_pos = 0;
	
    while(!is_quit) {
        {
            std::lock_guard<std::mutex> lock(io_mutex);
            moveCursorTo({0, y});
            clearCurrentLine();
            std::cout << "Enter input: " << std::flush;
        }

        moveCursorTo({13, y});
        std::getline(std::cin, input);

        {
            std::lock_guard<std::mutex> lock(io_mutex);
            
            console_pos++;
            moveCursorTo({0, y+console_pos});
            clearCurrentLine();
            
            if (std::regex_match(input, ping_regex)) 
			{
                std::cout << "pong";
            }
            else if (std::regex_match(input, match, text_regex)) 
			{
                marquee_text.set(match[1]);
                
                std::cout << "Changed text to " << marquee_text.get();
            } 
			else if (std::regex_match(input, match, speed_regex)) 
			{
                speed = std::stoi(match[1]);
                
                std::cout << "Changed speed to " << speed;
            } 
            else if (std::regex_match(input, clear_regex)) 
			{
				clearLines(y+1, y+1+console_pos);
				console_pos = 1;
				moveCursorTo({0, y+console_pos});
				
                std::cout << "Clear Console";
            } 
            else if (std::regex_match(input, quit_regex)) 
			{
				is_quit = true; 
				
                std::cout << "Goodbye";
            } 
			else {
                std::cout << "Invalid Command Line";
            	console_pos--;
            }
        }
    }
}

int main() {

	HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    
    GetConsoleScreenBufferInfo(h_console, &csbi);
    
    std::atomic_bool is_quit(false);
	std::atomic_int speed(1000);
	AtomicString marquee_text;
	marquee_text.set("Hello World in Marquee!");
	
    std::thread t1(marquee, std::ref(is_quit), std::ref(speed), std::ref(marquee_text), csbi.srWindow.Right - marquee_text.get().length(), csbi.srWindow.Bottom - 10);
    std::thread t2(console, std::ref(is_quit), std::ref(speed), std::ref(marquee_text), csbi.srWindow.Bottom - 9);

    t1.join();
    t2.join();
    
    return 0;
}
