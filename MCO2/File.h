#include "Lib.h"
#pragma once

class File {
    private:
        vector<string> file_content; // Stores the content of the file

    public:
        File() {}

        void Initialize(int num) {
            file_content.clear();

            vector<string> sample_code = {
                "DECLARE VAR_A",
                "DECLARE VAR_B",
                "VAR_A = " + to_string(num * 2152 % 100 + 5),
                "VAR_B = " + to_string(num * 3614 % 90 + 5),
                "PRINT VAR_A",
                "PRINT VAR_B",
                "DECLARE VAR_C = 3",
                "ADD VAR_C, VAR_A, VAR_B",
                "PRINT VAR_C",
                "FOR " + to_string(num * 2152 % 100 + 5),
                "ADD VAR_C, VAR_C, 1",
                "FOR " + to_string(num * 3614 % 90 + 5),
                "ADD VAR_C, VAR_C, 1",
                "END",
                "PRINT VAR_C",
                "END",
                "PRINT VAR_C",
                "PRINT 'Process completed successfully.'",
            };

            for (const string& line : sample_code) {
                file_content.push_back(line);
            }
        }

        vector<string> GetFileContent() const {
            return file_content;
        }

        string GetFileLine(int index) const {
            if (index >= 0 && index < file_content.size()) {
                return file_content[index];
            }
            return "";
        }

        int GetFileSize() {
            return file_content.size();
        }

        int GetBurstTime() {

            vector<int> stack;
            vector<int> internal_burst_time;
            stack.push_back(0);
            internal_burst_time.push_back(0);
            int stack_pointer = 0;

            regex re_for_loop(R"(FOR\s+(\d+))");
            smatch match;

            for (const string& line : file_content) {
                if(line.find("FOR") != string::npos || line.find("END") != string::npos) {
                    if (regex_search(line, match, re_for_loop)) {

                        int number = stoi(match[1]);

                        internal_burst_time[stack_pointer]++;
                        stack_pointer++;

                        if(stack.size() <= stack_pointer) {
                            stack.push_back(number);
                        } else {
                            stack[stack_pointer] = number;
                        }
                        if (internal_burst_time.size() <= stack_pointer) {
                            internal_burst_time.push_back(0);
                        } else {
                            internal_burst_time[stack_pointer] = 0;
                        }
                    }
                    else{
                        internal_burst_time[stack_pointer]++;
                        stack_pointer--;
                        internal_burst_time[stack_pointer] += internal_burst_time[stack_pointer + 1] * stack[stack_pointer + 1];
                    }
                } 
                else {
                    internal_burst_time[stack_pointer]++;
                }
            }

            return internal_burst_time[0];
        }
};