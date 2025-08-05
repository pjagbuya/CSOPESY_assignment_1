#pragma once

#include <queue>
#include <map>
#include <set>

#include "Process.h"

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

struct Core {
    int id;
    int pid;
    uint32_t time_quantum;
    uint32_t delay;
    int status;
};

#ifndef PAGE
#define PAGE
struct Page {
    int pid;
    int page_number;
    bool used_this_tick;
};
#endif 

class Scheduler {
    map<int, Process>& processes; 
    vector<Core> cores;
    queue<int> ready_queue;

    bool first_run;
    bool generate_processes = false;
    uint32_t process_counter = 0;
    uint32_t time_quantum;
    uint32_t freq;
    uint32_t min_ins;
    uint32_t max_ins;
    uint32_t delay;

    uint32_t max_overall_mem;
    uint32_t mem_per_frame;
    uint32_t min_mem_per_proc;
    uint32_t max_mem_per_proc;

    vector<Page> page_table;
    vector<Page> pages;

    uint32_t idle_ticks = 0;
    uint32_t active_ticks = 0;

    uint32_t paged_in = 0;
    uint32_t paged_out = 0;

    mutable mutex mtx;

public:
    Scheduler(map<int, Process>& processes, queue<int>& ready_queue, Config config): 
        processes(processes), 
        ready_queue(ready_queue), 
        cores(config.num_cpu), 
        time_quantum(config.quantum_cycles), 
        freq(config.batch_process_freq), 
        min_ins(config.min_ins), 
        max_ins(config.max_ins), 
        delay(config.delays_per_exec), 
        max_overall_mem(config.max_overall_mem),
        mem_per_frame(config.mem_per_frame),
        min_mem_per_proc(config.min_mem_per_proc),
        max_mem_per_proc(config.max_mem_per_proc),
        page_table(config.max_overall_mem / config.mem_per_frame),
        first_run(true) {

        for (int i = 0; i < config.num_cpu; ++i)
            cores[i] = Core{i, -1, 0, 0, 0};
        
        for (int i = 0; i < config.max_overall_mem / config.mem_per_frame; ++i)
            page_table[i] = Page{-1, -1, false};
        
    }

    void run_rr(uint32_t cpu_cycles) {

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
                    core.time_quantum++;
                    core.delay--;
                    core.status = 0;
                    idle_ticks++;

                    if (core.time_quantum >= time_quantum) {
                        ready_queue.push(core.pid);
                        remove_and_assign(core);
                    }
                    continue;
                }

                // Check if first frame of process is in memory...
                bool first_frame_in_mem = false;
                for (int i = 0; i < page_table.size(); i++) {
                    if (page_table[i].pid == core.pid && page_table[i].page_number == 0) {
                        first_frame_in_mem = true;
                        break;
                    }
                        
                }

                // If not, check if we can swap...
                if (!first_frame_in_mem) {
                    for (int i = 0; i < page_table.size(); i++) {
                        if (page_table[i].used_this_tick == false && page_table[i].pid != core.pid) {
                            if (page_table[i].pid != -1) paged_out++;
                            paged_in++;
                            first_frame_in_mem = true;
                            page_table[i].pid = core.pid;
                            page_table[i].page_number = 0;
                            page_table[i].used_this_tick = true;
                            break;
                        }
                    }
                }

                if (!first_frame_in_mem) {
                    core.status = 0;
                    core.time_quantum++;
                    idle_ticks++;
                    if (core.time_quantum >= time_quantum) {
                        ready_queue.push(core.pid);
                        remove_and_assign(core);
                    }
                    continue;
                }

                bool only_first_page_used = true;
                while (true) {
                    int page_number_required = process.tick(core.id, page_table, only_first_page_used);

                    if (page_number_required == -2) { // We crashed
                        active_ticks++;
                        remove_and_assign(core);
                        break;
                    } else if (page_number_required == -1) { // We successfully executed an instruction
                        core.status = 1;
                        core.time_quantum++;
                        active_ticks++;
                        if (only_first_page_used) { // We only used the first page
                            for (int i = 0; i < page_table.size(); i++) {
                                if (page_table[i].pid == core.pid && page_table[i].page_number == 0) {
                                    page_table[i].used_this_tick = true;
                                }
                            }
                        } else { // We used another page, READ and WRITE probably executed, also only 2 frames are needed for those instructions, the first page for instructions, and the second page for accessing the variable in the memory
                            for (int i = 0; i < page_table.size(); i++) {
                                if (page_table[i].pid == core.pid) {
                                    page_table[i].used_this_tick = true;
                                }
                            }                            
                        }
                        
                        if (process.is_done()) {
                            remove_and_assign(core);
                        } else if (core.time_quantum >= time_quantum) {
                            ready_queue.push(core.pid);
                            remove_and_assign(core);
                        }
                        break;
                    } else { // We need a new frame
                        bool swapped = false;
                        for (int i = 0; i < page_table.size(); i++) {
                            if (page_table[i].used_this_tick == false && page_table[i].pid != core.pid) {
                                if (page_table[i].pid != -1) paged_out++;

                                paged_in++;
                                page_table[i].pid = core.pid;
                                page_table[i].page_number = page_number_required;
                                page_table[i].used_this_tick = true;
                                swapped = true;
                                break;
                            }
                        }
                        if (!swapped) {
                            core.status = 0;
                            core.time_quantum++;
                            idle_ticks++;
                            if (core.time_quantum >= time_quantum) {
                                ready_queue.push(core.pid);
                                remove_and_assign(core);
                            }
                            break;
                        }                   
                    }
                }
            } else { 
                assign_next_process(core);
            }
        }

        // Reset swapped already
        for (int i = 0; i < page_table.size(); i++) {
            page_table[i].used_this_tick = false;
        }

        if (generate_processes && (cpu_cycles % freq == 0)) {
            create_random_process();
        }
    }

    void run_fcfs(uint32_t cpu_cycles) {

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
                    core.status = 0;
                    idle_ticks++;
                    continue;
                }
                
                // Check if first frame of process is in memory...
                bool first_frame_in_mem = false;
                for (int i = 0; i < page_table.size(); i++) {
                    if (page_table[i].pid == core.pid && page_table[i].page_number == 0) {
                        first_frame_in_mem = true;
                        break;
                    }       
                }

                // If not, check if we can swap...
                if (!first_frame_in_mem) {
                    for (int i = 0; i < page_table.size(); i++) {
                        if (page_table[i].used_this_tick == false && page_table[i].pid != core.pid) {
                            
                            if (page_table[i].pid != -1) paged_out++;

                            paged_in++;
                            first_frame_in_mem = true;
                            page_table[i].pid = core.pid;
                            page_table[i].page_number = 0;
                            page_table[i].used_this_tick = true;

                            break;
                        }
                    }
                }
                
                // If we failed to get the first frame, we move on to the next core
                if (!first_frame_in_mem) {
                    core.status = 0;
                    idle_ticks++;
                    continue;
                }
                
                // Check if other pages needed 
                bool only_first_page_used = true;
                while (true) {
                    int page_number_required = process.tick(core.id, page_table, only_first_page_used);

                    if (page_number_required == -2) { // We crashed
                        active_ticks++;
                        remove_and_assign(core);
                        break;
                    } else if (page_number_required == -1) { // We successfully executed an instruction
                        core.status = 1;
                        active_ticks++;
                        if (only_first_page_used) { // We only used the first page
                            for (int i = 0; i < page_table.size(); i++) {
                                if (page_table[i].pid == core.pid && page_table[i].page_number == 0) {
                                    page_table[i].used_this_tick = true;
                                }
                            }
                        } else { // We used another page, READ and WRITE probably executed, also only 2 frames are needed for those instructions, the first page for instructions, and the second page for accessing the variable in the memory
                            for (int i = 0; i < page_table.size(); i++) {
                                if (page_table[i].pid == core.pid) {
                                    page_table[i].used_this_tick = true;
                                }
                            }                            
                        }

                        if (process.is_done()) {
                            remove_and_assign(core);
                        } 
                        break;
                    } else { // We need a new frame
                        // throw runtime_error("Why is it trying to load this page: " + to_string(page_number_required));

                        bool swapped = false;
                        for (int i = 0; i < page_table.size(); i++) {
                            if (page_table[i].used_this_tick == false && page_table[i].pid != core.pid) {
                                if (page_table[i].pid != -1) paged_out++;

                                paged_in++;
                                page_table[i].pid = core.pid;
                                page_table[i].page_number = page_number_required;
                                page_table[i].used_this_tick = true;
                                swapped = true;
                                break;
                            }
                        }
                        if (!swapped) {
                            core.status = 0;
                            idle_ticks++;
                            break;
                        }                   
                    }
                }

            } else {
                assign_next_process(core);
            }
        }

        // Reset swapped already
        for (int i = 0; i < page_table.size(); i++) {
            page_table[i].used_this_tick = false;
        }

        if (generate_processes && (cpu_cycles % freq == 0)) {
            create_random_process();
        }
    }

    bool has_process(int pid) const {
        lock_guard<std::mutex> lock(mtx);
        return processes.count(pid) > 0;
    }

    void start_process_generation() {
        lock_guard<std::mutex> lock(mtx);
        generate_processes = true;
    }

    void stop_process_generation() {
        lock_guard<std::mutex> lock(mtx);
        generate_processes = false;
    }

    bool screen_create(int pid, int memory) {
        lock_guard<std::mutex> lock(mtx);

        if (processes.count(pid) > 0) {
            return false;
        }

        int num_instructions = min_ins + rand() % (max_ins - min_ins + 1);
        vector<string> program;
        
        int ins_ctr = 0;
        while (ins_ctr < num_instructions) {
            int type = 0 + rand() % 7;

            switch(type) {
                case 0: 
                    program.push_back("PRINT()"); 
                    ins_ctr++; 
                    break;
                case 1: 
                    program.push_back("DECLARE(x" + to_string(rand()%10) + ", " + to_string(rand()%100) + ")"); 
                    ins_ctr++;
                    break;
                case 2: 
                    program.push_back("SLEEP(" + to_string(rand()%3) + ")"); 
                    ins_ctr++;
                    break;
                case 3: 
                    if (ins_ctr + 6 > num_instructions) break;
                    program.push_back(generate_simple_for_loop());
                    ins_ctr += 6;
                    break;
                case 4: 
                    if (ins_ctr + 10 > num_instructions) break;
                    program.push_back(generate_nested_for_loop());
                    ins_ctr += 10;
                    break;
                case 5:
                    break; // TODO
                case 6:
                    break; // TODO
            }
        }

        processes.emplace(pid, Process(pid, program, memory, mem_per_frame));
        ready_queue.push(pid);

        int frame_count = (memory + mem_per_frame - 1) / mem_per_frame;

        for (int i = 0; i < frame_count; i++) {
            pages.push_back(Page{pid, i, false});
        }

        return true;

    }

    bool screen_custom(int pid, vector<string>& program, int memory) {
        lock_guard<std::mutex> lock(mtx);

        if (processes.count(pid) > 0) {
            return false;
        }

        processes.emplace(pid, Process(pid, program, memory, mem_per_frame));
        ready_queue.push(pid);

        int frame_count = (memory + mem_per_frame - 1) / mem_per_frame;

        for (int i = 0; i < frame_count; i++) {
            pages.push_back(Page{pid, i, false});
        }

        return true;

    }

    void process_smi(int pid) const {
        lock_guard<mutex> lock(mtx);

        if (!processes.count(pid)) {
            cout << "process_" << pid << " not found.\n";
            return;
        }

        const Process& process = processes.at(pid);

        cout << "----------------------------------------\n";
        cout << "Process name: " << "process_" << pid << endl;
        cout << "ID: " << pid << endl;
        cout << "Logs: \n";
        process.print_output();

        if (!process.is_done()) {       
            cout << "\nCurrent instruction line: " << process.current_instruction() << endl;
            cout << "Lines of code: " << process.total_instructions() << endl;
        } else if (process.is_crashed()) {
            cout << "Crashed!\n";
        } else {
            cout << "Finished!\n";
        }
    
        cout << "----------------------------------------\n";
    }

    void vmstat() const {
        lock_guard<mutex> lock(mtx);

        uint32_t total_memory = max_overall_mem;
        uint32_t frame_size = mem_per_frame;
        uint32_t total_frames = total_memory / frame_size;

        uint32_t used_memory = 0;
        for (const auto& page : page_table) {
            if (page.pid != -1)
                used_memory += frame_size;
        }

        uint32_t free_memory = total_memory - used_memory;
        uint32_t total_cpu_ticks = idle_ticks + active_ticks;

        // Output the stats
        cout << "----------------------------------------\n";
        cout << "VMSTAT\n";
        cout << "----------------------------------------\n";
        cout << "Total memory:\t\t" << total_memory << " bytes\n";
        cout << "Used memory:\t\t" << used_memory << " bytes\n";
        cout << "Free memory:\t\t" << free_memory << " bytes\n";
        cout << "Idle CPU ticks:\t\t" << idle_ticks << "\n";
        cout << "Active CPU ticks:\t" << active_ticks << "\n";
        cout << "Total CPU ticks:\t" << total_cpu_ticks << "\n";
        cout << "Pages paged in:\t\t" << paged_in << "\n";
        cout << "Pages paged out:\t" << paged_out << "\n";
        cout << "----------------------------------------\n";
    }

    void print_process_summary() const {
        lock_guard<mutex> lock(mtx);

        set<int> waiting_pids;
        queue<int> temp_queue = ready_queue;

        while (!temp_queue.empty()) {
            waiting_pids.insert(temp_queue.front());
            temp_queue.pop();
        }

        int used_cores = 0;
        for (const auto& core : cores) {
            if (core.pid != -1 && core.status == 1)
                used_cores++;
        }

        int total_cores = cores.size();
        int idle_cores = total_cores - used_cores;
        int utilization_percent = (total_cores == 0) ? 0 : (used_cores * 100) / total_cores;

        // Memory usage
        uint32_t used_memory = 0;
        int used_frames = 0;
        for (const auto& page : page_table) {
            if (page.pid != -1) {
                used_memory += mem_per_frame;
                used_frames++;
            }
        }

        uint32_t total_memory = max_overall_mem;
        int total_frames = total_memory / mem_per_frame;
        int memory_util_percent = (total_memory == 0) ? 0 : (used_memory * 100) / total_memory;

        cout << "\nCPU Utilization:\t" << utilization_percent << "%\n";
        cout << "Cores used:\t\t" << used_cores << "\n";
        cout << "Cores available:\t" << idle_cores << "\n\n";

        cout << "Memory Usage:\t\t" << used_memory << " bytes / " << total_memory << " bytes\n";
        cout << "Memory Utilization:\t" << memory_util_percent << "%\n\n";

        cout << "Memory Frames:\t\t" << used_frames << " / " << total_frames << "\n";
        for (int i = 0; i < page_table.size(); ++i) {
            const auto& page = page_table[i];
            if (page.pid != -1) {
                cout << "Frame " << i << ": process_" << page.pid
                    << "\tPage Number: " << page.page_number << "\n";
            }
        }

        cout << "\n--------------------------------------------------------------------------------";
        cout << "\nRunning processes:\n";
        for (int i = 0; i < cores.size(); ++i) {
            const Core& core = cores[i];
            if (core.pid != -1) {
                const Process& process = processes.at(core.pid);
                cout << "process_" << core.pid << "\t"
                        << "(" << process.arrival_time() << ")\t"
                        << "Core " << i << "\t"
                        << process.current_instruction() << "/" << process.total_instructions();

                if (core.status == 0) {
                    cout << "      \tIdle";
                } else if (core.status == 1) {
                    cout << "      \tExecuting";
                }
                cout << "\n";
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

        cout << "\nCrashed processes:\n";
        for (const auto& [pid, process] : processes) {
            if (process.is_crashed() &&
                !waiting_pids.count(pid) &&
                none_of(cores.begin(), cores.end(), [&](const Core& c) { return c.pid == pid; })) {
                cout << "process_" << pid << "\t"
                        << "(" << process.arrival_time() << ")\t\t"
                        << process.current_instruction() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        cout << "--------------------------------------------------------------------------------\n";
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

        int used_cores = 0;
        for (const auto& core : cores) {
            if (core.pid != -1 && core.status == 1)
                used_cores++;
        }

        int total_cores = cores.size();
        int idle_cores = total_cores - used_cores;
        int utilization_percent = (total_cores == 0) ? 0 : (used_cores * 100) / total_cores;

        // Memory usage
        uint32_t used_memory = 0;
        int used_frames = 0;
        for (const auto& page : page_table) {
            if (page.pid != -1) {
                used_memory += mem_per_frame;
                used_frames++;
            }
        }

        uint32_t total_memory = max_overall_mem;
        int total_frames = total_memory / mem_per_frame;
        int memory_util_percent = (total_memory == 0) ? 0 : (used_memory * 100) / total_memory;

        out << "\nCPU Utilization:\t" << utilization_percent << "%\n";
        out << "Cores used:\t\t" << used_cores << "\n";
        out << "Cores available:\t" << idle_cores << "\n\n";

        out << "Memory Usage:\t\t" << used_memory << " bytes / " << total_memory << " bytes\n";
        out << "Memory Utilization:\t" << memory_util_percent << "%\n\n";

        out << "Memory Frames:\t\t" << used_frames << " / " << total_frames << "\n";
        for (int i = 0; i < page_table.size(); ++i) {
            const auto& page = page_table[i];
            if (page.pid != -1) {
                out << "Frame " << i << ": process_" << page.pid
                    << "\tPage Number: " << page.page_number << "\n";
            }
        }

        out << "\n----------------------------------------";
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
            cout << "process_" << pid << "\t"
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

        out << "\nCrashed processes:\n";
        for (const auto& [pid, process] : processes) {
            if (process.is_crashed() &&
                !waiting_pids.count(pid) &&
                none_of(cores.begin(), cores.end(), [&](const Core& c) { return c.pid == pid; })) {
                out << "process_" << pid << "\t"
                        << "(" << process.arrival_time() << ")\t\t"
                        << process.total_instructions() << "/" << process.total_instructions()
                        << "\n";
            }
        }

        out << "----------------------------------------\n";
        out << endl;
    }

private:

    void remove_and_assign(Core& core) {
        core.pid = -1;
        core.time_quantum = 0;
        core.delay = delay;
        core.status = 0;
        assign_next_process(core);
    }

    void assign_next_process(Core& core) {
        if (!ready_queue.empty()) {
            int pid = ready_queue.front();
            ready_queue.pop();
            core.pid = pid;
            core.time_quantum = 0;
            core.delay = delay;
            core.status = 0;
        }
    }

    string generate_simple_for_loop() {
        return "FOR([PRINT(); DECLARE(x1, 2)], 3)";
    }

    string generate_nested_for_loop() {
        return "FOR([PRINT(); FOR([DECLARE(x2, 2); SLEEP(1)], 2)], 2)";
    }
        
    void create_random_process() {
        int pid = process_counter++;

        while(processes.count(pid)) {
            pid = process_counter++; // Loop until we don't have conflict anymore
        }

        int num_instructions = min_ins + rand() % (max_ins - min_ins + 1);
        vector<string> program;
        
        int ins_ctr = 0;
        while (ins_ctr < num_instructions) {
            int type = rand() % 7;

            switch(type) {
                case 0: 
                    program.push_back("PRINT()"); 
                    ins_ctr++; 
                    break;
                case 1: 
                    program.push_back("DECLARE(x" + to_string(rand()%10) + ", " + to_string(rand()%100) + ")"); 
                    ins_ctr++;
                    break;
                case 2: 
                    program.push_back("SLEEP(" + to_string(rand()%3) + ")"); 
                    ins_ctr++;
                    break;
                case 3: 
                    if (ins_ctr + 6 > num_instructions) break;
                    program.push_back(generate_simple_for_loop());
                    ins_ctr += 6;
                    break;
                case 4: 
                    if (ins_ctr + 10 > num_instructions) break;
                    program.push_back(generate_nested_for_loop());
                    ins_ctr += 10;
                    break;
                case 5:
                    break; // TODO
                case 6:
                    break; // TODO
            }
        }

        processes.emplace(pid, Process(pid, program, min_mem_per_proc, mem_per_frame));
        ready_queue.push(pid);

        int frame_count = (min_mem_per_proc + mem_per_frame - 1) / mem_per_frame;

        for (int i = 0; i < frame_count; i++) {
            pages.push_back(Page{pid, i, false});
        }

    }

};