#include "Lib.h"
#pragma once

#include "Process.h"

class Core {
    private:
        shared_ptr<Process> process;
        int id;

    public:
        Core(int id) { 
            this->id = id; 
            this->process = nullptr; 
        }

        int GetId() { return this->id; }
        void SetId(int id) { this->id = id; }
        shared_ptr<Process> GetProcess() { return this->process;}
        void SetProcess(shared_ptr<Process> process) {
            this->process = process;
            this->process->SetCoreId(this->id);
        }
        void ClearProcess() { this->process = nullptr; }

        void Start(){
            thread (&Core::Run, this).detach();
        }

        void Run() {
            while(true){
                auto current_process = this->process;
                if (current_process) {
                    current_process->Run();
                }
                Sleep(SLEEP_TIME);
            }
        }
};