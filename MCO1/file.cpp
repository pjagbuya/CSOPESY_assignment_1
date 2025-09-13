#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "hex.cpp"

class File{
    private:
        vector<Hex> instruction;

    public:
        File() {}

        void AddInstruction(const Hex& hex) {
            instruction.push_back(hex);
        }

        void RemoveInstruction(int index) {
            if (index >= 0 && index < instruction.size()) {
                instruction.erase(instruction.begin() + index);
            }
        }

        Hex GetInstruction(int index) {
            if (index >= 0 && index < instruction.size()) {
                return instruction[index];
            }
            throw out_of_range("Index out of range");
        }

        int GetInstructionCount() {
            return instruction.size();
        }

};