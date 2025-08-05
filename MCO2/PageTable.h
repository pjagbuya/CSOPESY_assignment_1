#include "Lib.h"
#pragma once

#include "Hex.h"

class PageTable {
    private:
        map<Hex, Hex> page_table; // Maps process ID to a vector of page numbers

    public:
        PageTable() {}

        void Initialize() {
        }

        void Run() {
        }

        void Shutdown() {
        }
};