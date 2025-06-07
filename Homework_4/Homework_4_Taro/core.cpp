#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "process.h"

class Core{
	private:
        int id;
        Process process;

    public:
        Core(int id) : process("", -1, 0, 0, 0, ""){
            this->id = id;
        }

        int GetID(){ return this->id; }
        void SetID( int id ){ this->id = id; }
        Process GetProcess(){ return this->process; }
        void SetProcess( Process process ){ this->process = process; }

        void RunProcess(){
            this->process.RunProcess(this->id);
        }

        
};

