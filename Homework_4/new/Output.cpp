#include "Lib.h"

#include "Lib.h"

class Output {
    private:
        HANDLE h_console;

    public:
        Output() {}

        void MoveCursorTo(COORD coord) { SetConsoleCursorPosition(this->h_console, coord); }
		void ClearCurrentLine() {
		    CONSOLE_SCREEN_BUFFER_INFO csbi;
		    DWORD count_written;
		    
		    GetConsoleScreenBufferInfo(this->h_console, &csbi);
		    COORD line_start = { 0, csbi.dwCursorPosition.Y };
		    FillConsoleOutputCharacter(this->h_console, ' ', csbi.dwSize.X, line_start, &count_written);
		}

        void ClearLine(int line_number) {
            COORD coord = { 0, (SHORT)line_number };
            MoveCursorTo(coord);
            ClearCurrentLine();
        }

        void Initialize() {
            this->h_console = GetStdHandle(STD_OUTPUT_HANDLE);
        }

        void Run(vector<string> output_list, string input, vector<string> cli_list) {
            int i = 0;
            
            for (string  line : output_list) {
                this->ClearLine(i);
                cout << line << endl;
                i++;
            }

            this->ClearLine(i);
            cout << input << endl;
            i++;

            for (string cli_line : cli_list) {
                this->ClearLine(i);
                cout << cli_line << endl;
                i++;
            }
        }

        void Shutdown() {
        }
};