#include <iostream>
#include <regex>
#include <thread>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <map>
#include <windows.h>

#include "Scheduler.h"

using namespace std;

#ifndef CONFIG
#define CONFIG
struct Config {
    int num_cpu;
    string scheduler;
    uint32_t quantum_cycles;
    uint32_t batch_process_freq;
    uint32_t min_ins;
    uint32_t max_ins;
    uint32_t delays_per_exec;
    uint32_t max_overall_mem;
    uint32_t mem_per_frame;
    uint32_t min_mem_per_proc;
    uint32_t max_mem_per_proc;
};
#endif

Config read_config(const std::string& filename);
void cow_init();
void cow();

Config config = read_config("config.txt");

map<int, Process> processes;
queue<int> ready_queue;

Scheduler scheduler(
    processes,
    ready_queue,
    config
);

thread scheduler_thread;
atomic<bool> scheduler_running = false;

uint32_t cpu_cycles = 0;

void screen(int pid) {

    while(true) {

        system("cls");
        cow();

        scheduler.process_smi(pid);

        cout << "Input command: ";
        string input;
        string dump;
        getline(cin, input);

        if (input == "process-smi") {
            system("cls");
            cow();
            scheduler.process_smi(pid);
        } else if (input == "exit") {
            break;
        } else {
            cout << "Command not recognized...\n";
        }

        cout << "Press Enter to continue...";
        getline(cin, dump);

    }
}

void start_scheduler() {
    
    while (true) {

        if (config.scheduler == "rr") {
            scheduler.run_rr(cpu_cycles);
        } else {
            scheduler.run_fcfs(cpu_cycles);
        }
        cpu_cycles++;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }

}

int main() {
    
    string input;
    smatch match;
    string dump;

    while(true) {
        system("cls");
        cow_init();
        cout << "Input command: ";
        getline(cin, input);

        if (input == "initialize") {
            cout << "Initializing operating system...";
            // scheduler_running = true;
            // scheduler_thread = thread(start_scheduler);
            // scheduler_thread.detach();
            Sleep(1000);
            break;
        } else if (input == "exit") {
            return 0;
        } else {
            cout << "Command not recognized...";
            Sleep(1000);
        }

    }

    while(true) {

        system("cls");
        cow();
        cout << "Input command: ";
        getline(cin, input);

        if (regex_match(input, match, regex(R"(screen -s (process_(\d+)) (\d+))"))) {
            int pid = stoi(match[2]);
            int mem_size = stoi(match[3]);

            if (scheduler.has_process(pid)) {
                cout << "Process already exists. Try screen -r process_" << pid << endl;
                cout << "Press Enter to continue...";
                getline(cin, dump);
            } else {
                if (mem_size < 64 || mem_size > 65536 || (mem_size & (mem_size - 1)) != 0) {
                    cout << mem_size;
                    cout << "Incorrect memory range" << endl;
                    cout << "Press Enter to continue";
                    getline(cin, dump);
                } else {
                    if (scheduler.screen_create(pid)) {
                        screen(pid);
                    } else {
                        cout << "Process already exists. Try screen -r process_" << pid << endl;
                        cout << "Press Enter to continue...";
                        getline(cin, dump);
                    }
                }
            }

        } else if (regex_match(input, match, regex(R"(screen -r (process_(\d+)))"))) {
            int pid = stoi(match[2]);
            if (scheduler.has_process(pid)) {
                screen(pid);
            } else {
                cout << "Process does not exist...\n";
                cout << "Press Enter to continue...";
                getline(cin, dump);
            }
        } else if (regex_match(input, match, regex(R"(screen -c (process_(\d+)) (\d+) \"(.*)\")"))) {
            int pid = stoi(match[2]);
            int mem_size = stoi(match[3]);
            vector<string> program;
            stringstream stream(match[4]);
            string instruction;

            while (getline(stream, instruction, ';')) {
                instruction.erase(0, instruction.find_first_not_of(" \t\n\r"));
                instruction.erase(instruction.find_last_not_of(" \t\n\r") + 1);
                if (!instruction.empty()) {
                    program.push_back(instruction);
                }
            }

            for (auto& line : program) {
                cout << line << endl;
            }

            getline(cin, dump);           

            if (scheduler.has_process(pid)) {
                cout << "Process already exists. Try screen -r process_" << pid << endl;
                cout << "Press Enter to continue...";
                getline(cin, dump);
            } else {
                if (mem_size < 64 || mem_size > 65536 || (mem_size & (mem_size - 1)) != 0) {
                    cout << "Incorrect memory range" << endl;
                    cout << "Press Enter to continue";
                    getline(cin, dump);
                } else {
                    if (scheduler.screen_custom(pid, program)) {
                        screen(pid);
                    } else {
                        cout << "Process already exists. Try screen -r process_" << pid << endl;
                        cout << "Press Enter to continue...";
                        getline(cin, dump);
                    }
                }
            }
        }
        else if (input == "screen -ls") {
            scheduler.print_process_summary();
            cout << "Press Enter to continue...";
            getline(cin, dump);
        } else if (input == "scheduler-start") {
            if (!scheduler_running) {
                scheduler_running = true;
                scheduler_thread = thread(start_scheduler);
                scheduler_thread.detach();
            }
            // scheduler.start_process_generation();
        } else if (input == "scheduler-stop") {
            // scheduler.stop_process_generation();
        } else if (input == "report-util") {
            scheduler.print_process_summary_to_file();
            cout << "Summary written to csopesy_log.txt\n";
            cout << "Press Enter to continue...";
            getline(cin, dump);
        } else if (input == "exit") {
            break;  
        } else {
            cout << "Command not recognized...\n";
            cout << "Press Enter to continue...";
            getline(cin, dump);
        }

    }

    return 0;
}

Config read_config(const string& filename) {
    Config config;
    ifstream file(filename);
    string key;
    string value;

    unordered_map<string, string> kv;

    while (file >> key >> value) {
        kv[key] = value;
    }

    config.num_cpu = stoi(kv["num-cpu"]);
    config.scheduler = kv["scheduler"];
    config.quantum_cycles = stoul(kv["quantum-cycles"]);
    config.batch_process_freq = stoul(kv["batch-process-freq"]);
    config.min_ins = stoul(kv["min-ins"]);
    config.max_ins = stoul(kv["max-ins"]);
    config.delays_per_exec = stoul(kv["delays-per-exec"]);
    config.max_overall_mem = stoul(kv["max-overall-mem"]);
    config.mem_per_frame = stoul(kv["mem-per-frame"]);
    config.min_mem_per_proc = stoul(kv["min-mem-per-proc"]);
    config.max_mem_per_proc = stoul(kv["max-mem-per-proc"]);

    return config;
}

void cow_init() {

string cow_init = R"(CowOS v1.0 Preboot 

            ^__^
            (oo)\_______
            (__)\       )\/\s
                ||----w |
                ||     ||

)";

    cout << cow_init;
}

void cow() {

string cow = R"(CowOS v1.0 Moolester Ready

            ^__^
            (oo)\_______
            (__)\       )\/\s
                ||----w |
                ||     ||

)";

    cout << cow;
}


