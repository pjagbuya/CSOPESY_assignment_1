#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>
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
        string IRAF;
        string IRDF;

        int microcode_counter;

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
            this->IRAF = "0x00000000";
            this->IRDF = "0x00000000";

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
        void SetIRAF(string value) { this->IRAF = value; }
        string GetIRAF() { return this->IRAF; }
        void SetIRDF(string value) { this->IRDF = value; }
        string GetIRDF() { return this->IRDF; }
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
        void SetMicrocodeCounter(int counter) { this->microcode_counter = counter; }
        int GetMicrocodeCounter() { return this->microcode_counter; }

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
            this->IRAF = "0x00000000";
            this->IRDF = "0x00000000";

            this->output_list = this->diagram;
            this->marquee_pos = 10;
            this->marquee_direction = 1;

            for(int i = 0; i < 256; i++){
                this->main_memory.push_back("0x00000000");
            }

            this->instruction_list.clear();

            this->microcode_counter = -1;

            this->main_memory[10] = "0x00001A1C"; // Example initialization
            this->main_memory[11] = "0x0002F2AC"; // Example initialization
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

            for(int i = 0; i < this->microcode.size(); i++){
                if(this->microcode_counter == i){
                    this->output_list[25 + i] = "> " + this->microcode[i];
                } else {
                    this->output_list[25 + i] = "  " + this->microcode[i];
                }
            }
        }

        void ClearMicrocode() {
            this->microcode.clear();
            this->microcode_counter = -1;
            for(int i = 25; i < this->output_list.size()-1; i++){
                this->output_list[i] = "";
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
            else if (reg_name == "MAR") return this->MAR;
            else if (reg_name == "MDR") return this->MDR;
            else if (reg_name == "Y") return this->Y;
            else if (reg_name == "ALU_B") return this->ALU_B;
            else if (reg_name == "ALU_A") return this->ALU_A;
            else if (reg_name == "ALU_OUT") return this->ALU_OUT;
            else if (reg_name == "Z") return this->Z;
            else if (reg_name == "CPU_BUS") return this->CPU_BUS;
            else if (reg_name == "IRAF") return this->IRAF;
            else if (reg_name == "IRDF") return this->IRDF;
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
            else if (reg_name == "MAR") this->MAR = value;
            else if (reg_name == "MDR") this->MDR = value;
            else if (reg_name == "Y") this->Y = value;
            else if (reg_name == "ALU_B") this->ALU_B = value;
            else if (reg_name == "ALU_A") this->ALU_A = value;
            else if (reg_name == "ALU_OUT") this->ALU_OUT = value;
            else if (reg_name == "Z") this->Z = value;
            else if (reg_name == "CPU_BUS") this->CPU_BUS = value;
            else if (reg_name == "IRAF") this->IRAF = value;
            else if (reg_name == "IRDF") this->IRDF = value;
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

        bool IsValidRegister(string input){
			if(input == "RAX" || input == "RBX" || input == "RCX" || input == "RDX" ||
			   input == "RDI" || input == "RSI" || input == "RBP" || input == "RSP") {
				return true;
			}
			return false;
		}

		bool IsValidHex(string input) {
			regex hexPattern("^0x[0-9A-Fa-f]{8}$");
			if (regex_match(input, hexPattern)){
				return true;
			}
			return false;
		}

		bool IsValidAddress(string input) {
			if (!IsValidHex(input)){
				return false;
			}
			int value = stoul(input.substr(2), nullptr, 16);
			return value <= 0x000000FF;
		}

		bool IsValidMMAccess(string input) {
			if(input.front() == '[' && input.back() == ']'){
				string inside = input.substr(1, input.size() - 2);
				if (IsValidAddress(inside)){
					return true;
				}
			}
			return false;
		}

        string HexAdd(string hex_a, string hex_b) {
            int a = stoi(hex_a, nullptr, 16);
            int b = stoi(hex_b, nullptr, 16);
            int c = a + b;

            // Convert back to hex string
            stringstream ss;
            ss << "0x"
            << uppercase 
            << setfill('0') 
            << setw(8) 
            << hex 
            << (unsigned int)c;
            return ss.str();
        }

        string HexSub(string hex_a, string hex_b) {
            int a = stoi(hex_a, nullptr, 16);
            int b = stoi(hex_b, nullptr, 16);
            int c = a - b;

            // Convert back to hex string
            stringstream ss;
            ss << "0x"
            << uppercase 
            << setfill('0') 
            << setw(8) 
            << hex 
            << (unsigned int)c;
            return ss.str();
        }

        string HexXor(string hex_a, string hex_b){
            int a = stoi(hex_a, nullptr, 16);
            int b = stoi(hex_b, nullptr, 16);
            int c = a ^ b;

            // Convert back to hex string
            stringstream ss;
            ss << "0x"
            << uppercase 
            << setfill('0') 
            << setw(8) 
            << hex 
            << (unsigned int)c;
            return ss.str();
        }

        void Decoder(){
                string instruction = this->microcode[this->microcode_counter];
                regex mov_pattern("^([A-Z]+)out,\\s*([A-Z]+)in$", regex_constants::icase);
                regex read_pattern("([A-Z]+)out,\\s*MARin,\\s*READ,\\s*WMFC", regex_constants::icase);
                regex write_1_pattern("([A-Z]+)out,\\s*MARin,\\s*WRITE,\\s*WMFC", regex_constants::icase);
                regex write_2_pattern("([A-Z]+)out,\\s*MDRin,\\s*WRITE,\\s*WMFC", regex_constants::icase);
                regex add_pattern("([A-Z]+)out,\\s*SELECT Y,\\s*ADD,\\s*Zin", regex_constants::icase);
                regex sub_pattern("([A-Z]+)out,\\s*SELECT Y,\\s*SUB,\\s*Zin", regex_constants::icase);
                regex xor_pattern("([A-Z]+)out,\\s*SELECT Y,\\s*XOR,\\s*Zin", regex_constants::icase);
                smatch match;
                if (regex_search(instruction, match, mov_pattern)) {
                    string out_reg = match[1];
                    string in_reg = match[2];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister(in_reg, this->GetRegister("CPU_BUS"));
                }
                else if (regex_search(instruction, match, read_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("MAR", this->GetRegister("CPU_BUS"));
                    this->WriteRegister("MDR", this->MemoryRead(this->GetRegister("MAR")));
                }
                else if (regex_search(instruction, match, write_1_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("MAR", this->GetRegister("CPU_BUS"));
                    this->MemoryWrite(this->GetRegister("MDR"), this->GetRegister("MAR"));
                }
                else if (regex_search(instruction, match, write_2_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("MDR", this->GetRegister("CPU_BUS"));
                    this->MemoryWrite(this->GetRegister("MDR"), this->GetRegister("MAR"));
                }
                else if (regex_search(instruction, match, add_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("ALU_A", this->GetRegister("CPU_BUS"));
                    this->WriteRegister("ALU_B", this->GetRegister("Y"));
                    this->WriteRegister("ALU_OUT", this->HexAdd(this->GetRegister("ALU_A"), this->GetRegister("ALU_B")));
                    this->WriteRegister("Z", this->GetRegister("ALU_OUT"));
                }
                else if (regex_search(instruction, match, sub_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("ALU_A", this->GetRegister("CPU_BUS"));
                    this->WriteRegister("ALU_B", this->GetRegister("Y"));
                    this->WriteRegister("ALU_OUT", this->HexSub(this->GetRegister("ALU_A"), this->GetRegister("ALU_B")));
                    this->WriteRegister("Z", this->GetRegister("ALU_OUT"));
                }
                else if (regex_search(instruction, match, xor_pattern)) {
                    string out_reg = match[1];
                    this->WriteRegister("CPU_BUS", this->GetRegister(out_reg));
                    this->WriteRegister("ALU_A", this->GetRegister("CPU_BUS"));
                    this->WriteRegister("ALU_B", this->GetRegister("Y"));
                    this->WriteRegister("ALU_OUT", this->HexXor(this->GetRegister("ALU_A"), this->GetRegister("ALU_B")));
                    this->WriteRegister("Z", this->GetRegister("ALU_OUT"));
                }
                else if(instruction == "END"){
                    this->ClearMicrocode();
                }
        }

        void Run(int clock_cycle){
            if(clock_cycle % 8 == 0){
                this->MoveMarqueePos();
            }
            if(clock_cycle % 80 == 0){
                if(this->microcode_counter < this->microcode.size() && this->microcode_counter != -1){
                    this->Decoder();
                    this->microcode_counter++;
                }
            }
            this->ReplaceValues();
        }
	
	};
		