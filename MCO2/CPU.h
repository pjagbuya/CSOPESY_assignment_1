#include "Lib.h"
#pragma once

#include "Core.h"

class CPU {
    private:
        vector<Core> cores;

    public:
        CPU() {}

        vector<Core> GetCores() { return this->cores;}
        void SetCores(vector<Core> cores) { this->cores = cores; }
        Core& GetCoreAtIndex(int index) { return this->cores[index]; }

        void SetProcessToCore(int core_index, shared_ptr<Process> process) {
            if (core_index >= 0 && core_index < cores.size()) {
                this->cores[core_index].SetProcess(process);
            }
        }

        void ClearProcessAtCore(int core_index) {
            if (core_index >= 0 && core_index < cores.size()) {
                this->cores[core_index].ClearProcess();
            }
        }

        void Initialize(int count) {
            for (int i = 0; i < count; ++i) {
                this->cores.push_back(Core(i));
            }
            for(auto& core : this->cores) {
                core.Start();
            }
        }

};