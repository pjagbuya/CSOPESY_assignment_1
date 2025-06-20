#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <regex>
#include <cstdint>
#include <algorithm>
#include <fstream>

using namespace std;

class Interpreter {
    unordered_map<string, uint16_t> vars;
    string process_name;
    vector<string> instructions;
    int instruction_pointer = 0;
    int sleep_ticks = 0;

public:
    Interpreter(string name, const vector<string>& program): process_name(name) {
        for (const auto& line : program) {
            unroll_instruction(line, instructions);
        }
    }

    bool tick() {
        if (sleep_ticks > 0) {
            --sleep_ticks;
            cout << "Slept" << endl;
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

    void debug_dump() const {
        string filename = process_name + "_debug_log.txt";
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
            cout << "Declare executed" << endl;
        } else if (regex_match(line, match, regex(R"(ADD\((\w+),\s*(\w+|\d+),\s*(\w+|\d+)\))"))) {
            handle_add(match[1], match[2], match[3]);
        } else if (regex_match(line, match, regex(R"(SUBTRACT\((\w+),\s*(\w+|\d+),\s*(\w+|\d+)\))"))) {
            handle_subtract(match[1], match[2], match[3]);
        } else if (regex_match(line, match, regex(R"(SLEEP\((\d+)\))"))) {
            sleep_ticks = stoi(match[1]);
            cout << "Sleep for " << sleep_ticks << " ticks" << endl;
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
            cout << result << endl;
        } else if (msg.empty()) { 
            cout << "Hello world from " << process_name << "!" << endl;
        } else {
            cout << msg << endl;
        }
    }

    void handle_add(const string& dest, const string& op1, const string& op2) {
        vars[dest] = clamp_u16(get_value(op1) + get_value(op2));
        cout << "Add executed" << endl;
    }

    void handle_subtract(const string& dest, const string& op1, const string& op2) {
        int val = get_value(op1) - get_value(op2);
        vars[dest] = (val < 0) ? 0 : clamp_u16(val);
        cout << "Subtract executed" << endl;
    }

    // How does sleeping work?
    // It uses ticks so I don't think the literal function Sleep is appropriate here since the scheduler is deterministic.
    // I assume I just delay the running of the next instruction by this amount
    //
    // UNUSED, left for posterity
    // void handle_sleep(int ticks) {
    //     cout << "Slept for " << ticks << " ticks" << endl;
    // };
    //
    // void handle_for(const string& raw_block, int repeats) {
    //     // Use ; as delimiter for instructions in for loop
    //     vector<string> loop_body = split(raw_block, ';');
    //     for (int i = 0; i < repeats; ++i) {
    //         for (const auto& line : loop_body) {
    //             execute(trim_copy(line));
    //         }
    //     }
    // }

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

int main() {

    vector<string> program = {
        "DECLARE(var1, 5)",
        "ADD(var1, var1, 10)",
        "SUBTRACT(var1, var1, 3)",
        "PRINT(\"Value from: \" +var1)",
        "SLEEP(2)",
        "FOR([PRINT(\"Value from: \" +var1); SUBTRACT(var1, var1, 1);], 2)",
        "FOR([PRINT(\"Value from: \" +var1); ADD(var1, var1, 1); FOR([PRINT(\"Value from: \" +var1); SUBTRACT(var1, var1, 1);], 2);], 1)",
        "PRINT()",
    };

    Interpreter interp("process_1", program);

    cout << "Total instructions after unrolling: " << interp.total_instructions() << endl;

    int tick = 0;
    while (!interp.is_done()) {
        cout << "Tick: " << tick << endl;
        interp.tick();
        tick++;
    }

    interp.debug_dump();

    return 0;

}
