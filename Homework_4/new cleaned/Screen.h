#include "Lib.h"
#pragma once

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

        void SetOutputList(vector<string> output_list) {
            this->output_list = output_list;
        }

        vector<string> GetOutputList() {
            return this->output_list;
        }

        void ClearOutputList() {
            this->output_list.clear();
        }

        void Initialize(string name) {
            this->output_list.push_back("Process name: " + name);
            this->output_list.push_back("Created at: " + this->GetTime());
            this->output_list.push_back("");
        }

        void Run(string output, int core_id = -1) {
            //this->output_list.push_back("At time : " + this->GetTime() + " using core " + to_string(core_id) + " : " + output);
        }

        void ReportUtil() {

        }
};