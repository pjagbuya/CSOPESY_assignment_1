#include "Lib.h"
#pragma once

#include "Hex.h"

struct SymbolInfo {
    string name;
    int value;
    int valid;

    SymbolInfo(string name, int value, int valid = 0)
        : name(name), value(value), valid(valid) {}
    
        SymbolInfo() : name(""), value(0), valid(0) {}

};

class SymbolTable {
    private:
        map<int, SymbolInfo> symbol_table; // Maps process ID to a vector of page numbers

    public:
        SymbolTable() {}

        void Initialize() {
            for(int i = 0; i < 64; i++) {
                this->symbol_table[i] = SymbolInfo("", 0, 0);
            }
        }

        void Insert(int addr, string name, int value) {
            this->symbol_table[addr] = SymbolInfo(name, value, 1);
        }

        void Update(int addr, int value) {
            if (this->symbol_table.find(addr) != this->symbol_table.end()) {
                this->symbol_table[addr].value = value;
            }
        }

        void Update(string name, int value) {
            for (auto& entry : this->symbol_table) {
                if (entry.second.name == name && entry.second.valid) {
                    entry.second.value = value;
                    return;
                }
            }
        }

        int Fetch(string name) {
            for (const auto& entry : this->symbol_table) {
                if (entry.second.name == name && entry.second.valid) {
                    return entry.second.value;
                }
            }
            return 0;
        }

        int GetSize() {
            int size = 0;
            for (const auto& entry : this->symbol_table) {
                if (entry.second.valid) {
                    size++;
                }
            }
            return size * 2;
        }
};