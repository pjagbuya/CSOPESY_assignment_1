#include "Lib.h"
#pragma once

#include "Core.h"

class CPU {
    private:
        vector<Core> cores;

    public:
        CPU() {}

        vector<Core> GetCores() {
            return cores;
        }

        void SetProcessToCore(int core_index, shared_ptr<Process> process) {
            if (core_index >= 0 && core_index < cores.size()) {
                cores[core_index].SetProcess(process);
            }
        }

        void ClearProcessAtCore(int core_index) {
            if (core_index >= 0 && core_index < cores.size()) {
                cores[core_index].SetProcess(nullptr);
            }
        }

        void SetCores(vector<Core> new_cores) {
            cores = new_cores;
        }

        void Initialize(int count) {
            for (int i = 0; i < count; ++i) {
                cores.push_back(Core(i));
            }
            for(auto& core : this->cores) {
                core.Start();
            }
        }

        Core GetCore(int index) {
            return cores[index];
        }

        void Run() {
            for (auto& core : cores) {
                core.Run();
            }
        }
};