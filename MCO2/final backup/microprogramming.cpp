#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
using namespace std;
#define ARR_SIZE 100

#include "screen.h"

class Microprogramming{
    private:
        string RAX;
        string RBX;
        string RCX;
        string RDX;
        string RDI;
        string RSI;
        string RBP;
        string RSP;
        string MAR;
        string MDR;
        string Y;
        string ALU_B;
        string ALU_A;
        string ALU_OUT;
        string Z;
        string CPU_BUS;

        vector<string> diagram = {
            "                 |            |",
            "+------------+   |            |    +------------+          +------------+------------+------------+------------+",
            "|     CU     |   |            |    |     MAR    |<---------| 0000  0000 | 0000  0001 | 0000  0002 | 0000  0003 |",
            "|            |-->|            |<-->| 0x00000000 |          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "+------------+   |            |    +------------+          +------------+------------+------------+------------+",
            "        ^        |            |    |     MDR    |<---------| 0000  0004 | 0000  0005 | 0000  0006 | 0000  0007 |",
            "        |        |            |<-->| 0x00000000 |          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "     +----+      |            |    +------------+          +------------+------------+------------+------------+",
            "     | IR |<-----|            |                            | 0000  0008 | 0000  0009 | 0000  000A | 0000  000B |",
            "     +----+      |            |    +------------+          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "                 |            |    |     Y      |          +------------+------------+------------+------------+",
            "+------------+   | 0x00000000 |--->| 0x00000000 |          | 0000  000C | 0000  000D | 0000  000E | 0000  000F |",
            "|     RAX    |   |            |    +------------+          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "| 0x00000000 |-->|            |              v<--4         +------------+------------+------------+------------+",
            "+------------+   |            |    +------------+          | 0000  0010 | 0000  0011 | 0000  0012 | 0000  0013 |",
            "|     RBX    |   |            |--->|  A      B  |          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "| 0x00000000 |-->|            |    |     ALU    |<--NOP    +------------+------------+------------+------------+",
            "+------------+   |            |    | 0x00000000 |          | 0000  0014 | 0000  0015 | 0000  0016 | 0000  0017 |",
            "|     RCX    |   |            |    +------------+          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "| 0x00000000 |-->|            |           v                +------------+------------+------------+------------+",
            "+------------+   |            |    +------------+          | 0000  0018 | 0000  0019 | 0000  001A | 0000  001B |",
            "|     RDX    |   |            |    |     Z      |          | 0x00000000 | 0x00000000 | 0x00000000 | 0x00000000 |",
            "| 0x00000000 |-->|            |<---| 0x00000000 |          +------------+------------+------------+------------+",
            "+------------+   |            |    +------------+          | 0000  001C | 0000  001D | 0000  001E | 0000  001F |",
            "----------------------------------------------------------------------------------------------------------------",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "",
            "----------------------------------------------------------------------------------------------------------------",
        };

        vector<string> output_list;
        vector<string> instruction_list;
        vector<string> main_memory;
        vector<string> microcode;

        int marquee_pos;
        int marquee_direction;
    
    public:
        Microprogramming(){
            this->RAX = "0x00000000";
            this->RBX = "0x00000000";
            this->RCX = "0x00000000";
            this->RDX = "0x00000000";
            this->RDI = "0x00000000";
            this->RSI = "0x00000000";
            this->RBP = "0x00000000";
            this->RSP = "0x00000000";
            this->MAR = "0x00000000";
            this->MDR = "0x00000000";
            this->Y = "0x00000000";
            this->ALU_B = "0x00000000";
            this->ALU_A = "0x00000000";
            this->ALU_OUT = "0x00000000";
            this->Z = "0x00000000";
            this->CPU_BUS = "0x00000000";

            this->output_list = this->diagram;

        }

        void SetRAX(string value) { this->RAX = value; }
        string GetRAX() { return this->RAX; }
        void SetRBX(string value) { this->RBX = value; }
        string GetRBX() { return this->RBX; }
        void SetRCX(string value) { this->RCX = value; }
        string GetRCX() { return this->RCX; }
        void SetRDX(string value) { this->RDX = value; }
        string GetRDX() { return this->RDX; }
        void SetRDI(string value) { this->RDI = value; }
        string GetRDI() { return this->RDI; }
        void SetRSI(string value) { this->RSI = value; }
        string GetRSI() { return this->RSI; }
        void SetRBP(string value) { this->RBP = value; }
        string GetRBP() { return this->RBP; }
        void SetRSP(string value) { this->RSP = value; }
        string GetRSP() { return this->RSP; }
        void SetMAR(string value) { this->MAR = value; }
        string GetMAR() { return this->MAR; }
        void SetMDR(string value) { this->MDR = value; }
        string GetMDR() { return this->MDR; }
        void SetY(string value) { this->Y = value; }
        string GetY() { return this->Y; }
        void SetALU_B(string value) { this->ALU_B = value; }
        string GetALU_B() { return this->ALU_B; }
        void SetALU_A(string value) { this->ALU_A = value; }
        string GetALU_A() { return this->ALU_A; }
        void SetALU_OUT(string value) { this->ALU_OUT = value; }
        string GetALU_OUT() { return this->ALU_OUT; }
        void SetZ(string value) { this->Z = value; }
        string GetZ() { return this->Z; }
        void SetCPU_BUS(string value) { this->CPU_BUS = value; }
        string GetCPU_BUS() { return this->CPU_BUS; }
        void SetDiagram(vector<string> diagram) { this->diagram = diagram; }
        vector<string> GetDiagram() { return this->diagram; }
        void SetOutputList(vector<string> output_list) { this->output_list = output_list; }
        vector<string> GetOutputList() { return this->output_list; }
        void SetInstructionList(vector<string> instruction_list) { this->instruction_list = instruction_list; }
        vector<string> GetInstructionList() { return this->instruction_list; }
        void SetMainMemory(vector<string> memory) { this->main_memory = memory; }
        vector<string> GetMainMemory() { return this->main_memory; }
        void SetMicrocode(vector<string> microcode) { this->microcode = microcode; }
        vector<string> GetMicrocode() { return this->microcode; }
        void SetMarqueePos(int pos) { this->marquee_pos = pos; }
        int GetMarqueePos() { return this->marquee_pos; }
        void SetMarqueeDirection(int direction) { this->marquee_direction = direction; }
        int GetMarqueeDirection() { return this->marquee_direction; }

        void Initialize(){
            this->RAX = "0x00000000";
            this->RBX = "0x00000000";
            this->RCX = "0x00000000";
            this->RDX = "0x00000000";
            this->RDI = "0x00000000";
            this->RSI = "0x00000000";
            this->RBP = "0x00000000";
            this->RSP = "0x00000000";
            this->MAR = "0x00000000";
            this->MDR = "0x00000000";
            this->Y = "0x00000000";
            this->ALU_B = "0x00000000";
            this->ALU_A = "0x00000000";
            this->ALU_OUT = "0x00000000";
            this->Z = "0x00000000";
            this->CPU_BUS = "0x00000000";

            this->output_list = this->diagram;
            this->marquee_pos = 10;
            this->marquee_direction = 1;

            for(int i = 0; i < 256; i++){
                this->main_memory.push_back("0x00000000");
            }

            this->instruction_list.clear();
        }

        void AddInstruction(string instruction) {
            this->instruction_list.push_back(instruction);
        }

        void ReplaceAt(string& line, int col, const string& value) {
            if (col + value.size() <= line.size()) {
                line.replace(col, value.size(), value);
            }
        }

        void ReplaceValues() {

            // CPU Registers
            ReplaceAt(this->output_list[13], 2, this->RAX);      // RAX
            ReplaceAt(this->output_list[16], 2, this->RBX);      // RBX
            ReplaceAt(this->output_list[19], 2, this->RCX);      // RCX
            ReplaceAt(this->output_list[22], 2, this->RDX);      // RDX

            // Internal Components
            ReplaceAt(this->output_list[3], 37, this->MAR);      // MAR
            ReplaceAt(this->output_list[6], 37, this->MDR);      // MDR
            ReplaceAt(this->output_list[11], 37, this->Y);       // Y
            ReplaceAt(this->output_list[17], 37, this->ALU_OUT); // ALU OUT (ALU)
            ReplaceAt(this->output_list[22], 37, this->Z);       // Z

            // CPU Bus
            ReplaceAt(this->output_list[this->marquee_pos], 19, this->CPU_BUS);
            ReplaceAt(this->output_list[this->marquee_pos - this->marquee_direction], 19, "          ");

            // Main Memory
            int mem_line = 3;
            int index = 0;

            while (mem_line < this->output_list.size() - 14 && index + 3 < this->main_memory.size()) {
                ReplaceAt(this->output_list[mem_line], 61, this->main_memory[index]);
                ReplaceAt(this->output_list[mem_line], 74, this->main_memory[index + 1]);
                ReplaceAt(this->output_list[mem_line], 87, this->main_memory[index + 2]);
                ReplaceAt(this->output_list[mem_line], 100, this->main_memory[index + 3]);

                index += 4;
                mem_line += 3;
            }
        }

        void MoveMarqueePos() {
            if(this->marquee_pos == 0){
                this->marquee_direction = 1;
            } else if(this->marquee_pos == this->output_list[0].size() - 8){
                this->marquee_direction = -1;
            }
            this->marquee_pos += this->marquee_direction;
        }

        string GetRegister(string reg_name) {
            if (reg_name == "RAX") return this->RAX;
            else if (reg_name == "RBX") return this->RBX;
            else if (reg_name == "RCX") return this->RCX;
            else if (reg_name == "RDX") return this->RDX;
            else if (reg_name == "RDI") return this->RDI;
            else if (reg_name == "RSI") return this->RSI;
            else if (reg_name == "RBP") return this->RBP;
            else if (reg_name == "RSP") return this->RSP;
            return "";
        }

        void WriteRegister(string reg_name, string value) {
            if (reg_name == "RAX") this->RAX = value;
            else if (reg_name == "RBX") this->RBX = value;
            else if (reg_name == "RCX") this->RCX = value;
            else if (reg_name == "RDX") this->RDX = value;
            else if (reg_name == "RDI") this->RDI = value;
            else if (reg_name == "RSI") this->RSI = value;
            else if (reg_name == "RBP") this->RBP = value;
            else if (reg_name == "RSP") this->RSP = value;
        }

        string MemoryRead(string address) {
            int int_address = stoi(address, nullptr, 16);
            if(int_address >= 0 && int_address < this->main_memory.size()){
                return this->main_memory[int_address];
            }
        }

        void MemoryWrite(string data, string address){
            int int_address = stoi(address, nullptr, 16);
            if(int_address >= 0 && int_address < this->main_memory.size()){
                this->main_memory[int_address] = data;
            }
        }

        void Decoder(int action, string operand1, string operand2){

        }

        void Run(int clock_cycle){
            if(clock_cycle % 8 == 0){
                this->MoveMarqueePos();
            }
            this->ReplaceValues();
        }
	
	};
		