#include "Lib.h"

#include "Core.cpp"

class CPU {
    private:
        vector<Core> cores;

    public:
        CPU() {}

        void Initialize(int count) {
            for (int i = 0; i < count; ++i) {
                cores.push_back(Core());
            }
        }

        Core GetCore(int index) {
            if (index < 0 || index >= cores.size()) {
            }
            return cores[index];
        }

        void Run() {
            for (auto& core : cores) {
                core.Run();
            }
        }
};