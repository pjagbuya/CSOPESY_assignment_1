#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

class Process {

    unordered_map<string, uint16_t> vars;
    int pid;
    vector<string> instructions;
    int instruction_pointer = 0;
    int sleep_ticks = 0;
    int core = -1;

    chrono::system_clock::time_point timestamp;

    ostringstream buffer;

public:
    Process(int pid, const vector<string>& program): pid(pid), timestamp(chrono::system_clock::now())  {
        for (const auto& line : program) {
            unroll_instruction(line, instructions);
        }
    }

    bool tick(int core) {

        this->core = core; // OMG

        if (sleep_ticks > 0) {
            --sleep_ticks;
            return false; // Skipping execution due to sleep
        }

        if (instruction_pointer >= instructions.size()) {
            return false; // No more instructions
        }

        execute(instructions[instruction_pointer]);
        instruction_pointer++;
        return true; // Instruction executed
    }

    bool is_done() const {
        return instruction_pointer >= instructions.size() && sleep_ticks == 0;
    }

    bool is_sleeping() const {
        return sleep_ticks > 0;
    }

    int total_instructions() const {
        return instructions.size();
    }

    int current_instruction() const {
        return instruction_pointer;
    }

    string arrival_time() const {
        time_t t = chrono::system_clock::to_time_t(timestamp);
        char buf[100];
        strftime(buf, sizeof(buf), "%m-%d-%Y %H:%M:%S", localtime(&t));
        return string(buf);
    }

    void print_output() const {
        cout << buffer.str() << endl;
    }

    void debug_dump() const {
        string filename = "process_" + to_string(pid)+ "_debug_log.txt";
        ofstream log(filename, ios::trunc);

        log << "==== DEBUG DUMP ====\n";
        log << "Instruction Pointer: " << instruction_pointer << "\n";
        log << "Total Instructions: " << instructions.size() << "\n";
        log << "Instructions:\n";
        for (int i = 0; i < instructions.size(); ++i) {
            log << "  [" << i << "] " << instructions[i] << "\n";
        }

        log << "\nVariable Map:\n";
        for (const auto& [var, val] : vars) {
            log << "  " << var << " = " << val << "\n";
        }

        log << "\nSleep Ticks Remaining: " << sleep_ticks << "\n";
        log << "===============================\n\n";
        log.close();
    }

private:
    void unroll_instruction(const string& line, vector<string>& output) {
        smatch match;
        if (regex_match(line, match, regex(R"(FOR\(\[(.*)\],\s*(\d+)\))"))) {
            string block = match[1];
            int repeats = stoi(match[2]);
            vector<string> loop_body = split(block, ';');
            for (int i = 0; i < repeats; i++) {
                for (const string& cmd : loop_body) {
                    unroll_instruction(trim_copy(cmd), output);
                }
            }
        } else {
            output.push_back(line);
        }
    }    

    void execute(const string& line) {
        smatch match;
        if (regex_match(line, match, regex(R"(PRINT\((.*)\))"))) {
            handle_print(match[1]);
        } else if (regex_match(line, match, regex(R"(DECLARE\((\w+),\s*(\d+)\))"))) {
            vars[match[1]] = clamp_u16(stoi(match[2]));
        } else if (regex_match(line, match, regex(R"(ADD\((\w+),\s*(\w+|\d+),\s*(\w+|\d+)\))"))) {
            handle_add(match[1], match[2], match[3]);
        } else if (regex_match(line, match, regex(R"(SUBTRACT\((\w+),\s*(\w+|\d+),\s*(\w+|\d+)\))"))) {
            handle_subtract(match[1], match[2], match[3]);
        } else if (regex_match(line, match, regex(R"(SLEEP\((\d+)\))"))) {
            sleep_ticks = stoi(match[1]);
        }
    }

    void handle_print(const string& msg) {
        // For "Value from: " +x
        if (msg.find('+') != string::npos) {
            auto parts = split(msg, '+');
            string result;
            for (auto& part : parts) {
                trim(part);
                if (part.front() == '"') {
                    result += strip_quotes(part);
                } else {
                    result += to_string(vars[part]);
                }
            }
            buffer << result << endl;
        } else if (msg.empty()) {
            time_t t = chrono::system_clock::to_time_t(chrono::system_clock::now());
            char buf[100];
            strftime(buf, sizeof(buf), "%m-%d-%Y %H:%M:%S", localtime(&t));

            buffer << "(" << string(buf) << ") " << "Core:" << core << " Hello world from " << "process_" << pid << "!" << endl;
        } else {
            buffer << msg << endl;
        }
    }

    void handle_add(const string& dest, const string& op1, const string& op2) {
        vars[dest] = clamp_u16(get_value(op1) + get_value(op2));
    }

    void handle_subtract(const string& dest, const string& op1, const string& op2) {
        int val = get_value(op1) - get_value(op2);
        vars[dest] = (val < 0) ? 0 : clamp_u16(val);
    }

    // We disallow starting a variable with a number so we can just check the first character.
    uint16_t get_value(const string& s) {
        if (isdigit(s[0])) return clamp_u16(stoi(s));
        return vars[s];
    }

    // Val should be int so that we don't have to worry about underflow or overflow from addition and subtraction, it gets handled here.
    uint16_t clamp_u16(int val) {
        return min(max(0, val), 65535);
    }
    
    // For nested loops, so that we can split them per level 
    vector<string> split(const string& block, char delimiter) {
        vector<string> result;
        string current;
        int bracket_depth = 0;

        for (size_t i = 0; i < block.size(); ++i) {
            char c = block[i];

            if (c == '[') bracket_depth++;
            else if (c == ']') bracket_depth--;

            if (c == delimiter && bracket_depth == 0) {
                result.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        if (!current.empty()) result.push_back(current);
        return result;
    }

    // If we want to trim but not modify the original string
    string trim_copy(string s) {
        trim(s);
        return s;
    }

    void trim(string& s) {

        // Trim whitespace chars starting from the left up to the first non-whitespace char.
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
            return !isspace(ch);
        }));

        // Do this also starting from the right going left up to the last non-whitespace char.
        s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
            return !isspace(ch);
        }).base(), s.end());

    }

    string strip_quotes(const string& s) {
        return s.substr(1, s.size() - 2);
    }
};

// ===TEST===
// int main() {
//
//     vector<string> program = {
//         "DECLARE(var1, 5)",
//         "ADD(var1, var1, 10)",
//         "SUBTRACT(var1, var1, 3)",
//         "PRINT(\"Value from: \" +var1)",
//         "SLEEP(2)",
//         "FOR([PRINT(\"Value from: \" +var1); SUBTRACT(var1, var1, 1);], 2)",
//         "FOR([PRINT(\"Value from: \" +var1); ADD(var1, var1, 1); FOR([PRINT(\"Value from: \" +var1); SUBTRACT(var1, var1, 1);], 2);], 1)",
//         "PRINT()",
//     };
//
//     vector<string> program(100, "PRINT()");
//    
//     Interpreter interp("process_1", program);
//
//     cout << "Total instructions after unrolling: " << interp.total_instructions() << endl;
//
//     int tick = 0;
//     while (!interp.is_done()) {
//         cout << "Tick: " << tick << endl;
//         interp.tick();
//         tick++;
//     }
//
//     interp.debug_dump();
//
//     return 0;
//
// }
