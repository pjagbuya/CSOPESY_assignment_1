#include "Lib.h"

class Screen {
    private:
        vector<string> output_list;

    public:
        Screen() {}

        string GetTime(){
			time_t timestamp;
			time(&timestamp);
			char* time_str = ctime(&timestamp);
			string time_string(time_str);
			time_string.erase(time_string.length() - 1);
			return time_string;
		}

        void Initialize(string name) {
            this->output_list.push_back("Process name: " + name);
            this->output_list.push_back("Created at: " + this->GetTime());
            this->output_list.push_back("");
        }

        void Run(string output) {
            this->output_list.push_back(output);
        }

        void ReportUtil() {

        }
};