#pragma once

#include <queue>
#include <map>
#include <set>

#include "Process.h"

struct Core {
    int pid;
    uint32_t time_quantum;
    uint32_t delay;
};

class Scheduler {
    map<int, Process>& processes; 
    vector<Core> cores;
    queue<int> ready_queue;

    bool first_run;
    uint32_t time_quantum;
    uint32_t delay;

    mutable mutex mtx;

public:
    Scheduler(map<int, Process>& processes, queue<int>& ready_queue, int num_cores, uint32_t time_quantum, uint32_t delay)
        : processes(processes), ready_queue(ready_queue), cores(num_cores, Core{-1, 0, 0}), time_quantum(time_quantum), delay(delay), first_run(true) {}

    void run_rr() {

        lock_guard<mutex> lock(mtx);

        // On the first run, we need to populate the cores with processes first.
        if (first_run) {
            for (Core& core : cores) {
                assign_next_process(core);
            }
            first_run = false;
        }

        // Loop through the cores, execute the next instruction for each process, remove finished processes
        // preempt processes exceeding time quantum, and assign new processes to each empty core.
        for (Core& core : cores) {
            if (core.pid != -1) {
                Process& process = processes.at(core.pid);

                // Check if core is delayed
                if (core.delay > 0) {
                    core.time_quantum++; // Increment time quantum of process and decrement delay
                    core.delay--;
                    // Remove process if it exceeded the time quantum
                    if (core.time_quantum >= time_quantum) {
                        ready_queue.push(core.pid);
                        remove_and_assign(core);
                    }
                    continue;
                }

                process.tick();
                core.time_quantum++;

                if (process.is_done()) {
                    remove_and_assign(core);
                } else if (core.time_quantum >= time_quantum) {
                    ready_queue.push(core.pid);
                    remove_and_assign(core);
                }
            } else { // We probably shouldn't reach this ever since we assign a new process immediately after a core is emptied.  
                assign_next_process(core);
            }
        }
    }

    void run_fcfs() {

        lock_guard<mutex> lock(mtx);

        if (first_run) {
            for (Core& core : cores) {
                assign_next_process(core);
            }
            first_run = false;
        } 

        for (Core& core : cores) {
            if (core.pid != -1) {

                Process& process = processes.at(core.pid);

                if (core.delay > 0) {
                    core.delay--;
                    continue;
                }
                
                process.tick();

                if (process.is_done()) {
                    core.pid = -1;
                    core.delay = delay;
                    assign_next_process(core);
                } 

            } else {
                assign_next_process(core);
            }
        }
    }

    void print_process_summary() const {
        lock_guard<mutex> lock(mtx);

        set<int> waiting_pids;
        queue<int> temp_queue = ready_queue;

        while (!temp_queue.empty()) {
            waiting_pids.insert(temp_queue.front());
            temp_queue.pop();
        }

        cout << "\nRunning processes:\n";
        for (int i = 0; i < cores.size(); ++i) {
            const Core& core = cores[i];
            if (core.pid != -1) {
                const Process& process = processes.at(core.pid);
                cout << "process_" << core.pid << "\t"
                        << "(" << process.arrival_time() << ")\t"
                        << "Core " << i << "\t"
                        << process.current_instruction() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        cout << "\nWaiting processes:\n";
        for (int pid : waiting_pids) {
            const Process& process = processes.at(pid);
            cout << "process_" << pid << "\t"
                    << "(" << process.arrival_time() << ")\t\t"
                    << process.current_instruction() << "/" << process.total_instructions()
                    << "\n";
        }

        cout << "\nFinished processes:\n";
        for (const auto& [pid, process] : processes) {
            if (process.is_done() &&
                !waiting_pids.count(pid) &&
                none_of(cores.begin(), cores.end(), [&](const Core& c) { return c.pid == pid; })) {
                cout << "process_" << pid << "\t"
                        << "(" << process.arrival_time() << ")\t\t"
                        << process.total_instructions() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        cout << endl;
    }


    void print_process_summary_to_file(const string& filename = "csopesy_log.txt") const {
        lock_guard<mutex> lock(mtx);

        ofstream out(filename);
        if (!out.is_open()) {
            cerr << "Failed to open file: " << filename << "\n";
            return;
        }    

        set<int> waiting_pids;
        queue<int> temp_queue = ready_queue;

        while (!temp_queue.empty()) {
            waiting_pids.insert(temp_queue.front());
            temp_queue.pop();
        }

        out << "\nRunning processes:\n";
        for (int i = 0; i < cores.size(); ++i) {
            const Core& core = cores[i];
            if (core.pid != -1) {
                const Process& process = processes.at(core.pid);
                out << "process_" << core.pid << "\t"
                        << "(" << process.arrival_time() << ")\t"
                        << "Core " << i << "\t"
                        << process.current_instruction() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        out << "\nWaiting processes:\n";
        for (int pid : waiting_pids) {
            const Process& process = processes.at(pid);
            out << "process_" << pid << "\t"
                    << "(" << process.arrival_time() << ")\t\t"
                    << process.current_instruction() << "/" << process.total_instructions()
                    << "\n";
        }

        out << "\nFinished processes:\n";
        for (const auto& [pid, process] : processes) {
            if (process.is_done() &&
                !waiting_pids.count(pid) &&
                none_of(cores.begin(), cores.end(), [&](const Core& c) { return c.pid == pid; })) {
                out << "process_" << pid << "\t"
                        << "(" << process.arrival_time() << ")\t\t"
                        << process.total_instructions() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        out << endl;
    }

private:

    void remove_and_assign(Core& core) {
        core.pid = -1;
        core.time_quantum = 0;
        core.delay = delay;
        assign_next_process(core);
    }

    void assign_next_process(Core& core) {
        if (!ready_queue.empty()) {
            int pid = ready_queue.front();
            ready_queue.pop();
            core.pid = pid;
            core.time_quantum = 0;
            core.delay = delay;
        }
    }

};