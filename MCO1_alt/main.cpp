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

struct Config {
    int num_cpu;
    string scheduler;
    uint32_t quantum_cycles;
    uint32_t batch_process_freq;
    uint32_t min_ins;
    uint32_t max_ins;
    uint32_t delays_per_exec;
};

Config read_config(const std::string& filename);
void cow_init();
void cow();

Config config = read_config("config.txt");

map<int, Process> processes;
queue<int> ready_queue;
int num_processes = 0;

Scheduler scheduler(
    processes,
    ready_queue,
    config.num_cpu,
    config.quantum_cycles,
    config.delays_per_exec
);

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
            scheduler.run_rr();
        } else {
            scheduler.run_fcfs();
        }
        cpu_cycles++;

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

        if (regex_match(input, match, regex(R"(screen -s (process_(\d+)))"))) {
            int pid = stoi(match[2]);
            if (scheduler.has_process(pid)) {
                cout << "Process already exists. Try screen -r process_" << pid << endl;
                cout << "Press Enter to continue...";
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
        } else if (regex_match(input, match, regex(R"(screen -r (process_(\d+)))"))) {
            int pid = stoi(match[2]);
            if (scheduler.has_process(pid)) {
                screen(pid);
            } else {
                cout << "Process does not exist...\n";
                cout << "Press Enter to continue...";
                getline(cin, dump);
            }
        } else if (input == "screen -ls") {
            scheduler.print_process_summary();
            cout << "Press Enter to continue...";
            getline(cin, dump);
        } else if (input == "scheduler-start") {
            cout << "Starting scheduler";
            thread scheduler(start_scheduler);
        } else if (input == "scheduler-stop") {
            cout << "Stopping scheduler";
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


