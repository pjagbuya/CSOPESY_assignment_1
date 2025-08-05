#include "Lib.h"
#pragma once

#include "Hex.h"

class LogicalMemory {
    private:
        map<Hex, Hex> logical_memory;
        int memory_size = 0;

    public:
        LogicalMemory() {}

        bool Insert(Hex addr, Hex data) {
            if(addr.ToInt() < 0 || addr.ToInt() >= memory_size) {
                return false; // Address out of bounds
            }
            logical_memory[addr] = data;
        }

        void Remove(Hex addr) {
            if(logical_memory.find(addr) != logical_memory.end()) {
                logical_memory.erase(addr);
            }
        }

        void Clear() {
            logical_memory.clear();
        }
};