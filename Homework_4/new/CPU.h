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

        void SetCores(vector<Core> new_cores) {
            cores = new_cores;
        }

        void Initialize(int count) {
            for (int i = 0; i < count; ++i) {
                cores.push_back(Core(i));
                cores[i].Start();
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