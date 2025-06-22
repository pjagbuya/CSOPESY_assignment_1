#include "Lib.h"
#pragma once

#include "Process.h"

class Core {
    private:
        shared_ptr<Process> process;
        int id;

    public:
        Core(int id) { this->id = id; }

        shared_ptr<Process> GetProcess() {
            return this->process;
        }

        void SetProcess(shared_ptr<Process> new_process) {
            this->process = new_process;
            this->process->SetCoreId(this->id);
        }

        shared_ptr<Process> ContextSwitch(shared_ptr<Process> new_process) {
            shared_ptr<Process> old_process = process;
            process = new_process;
            return old_process;
        }

        void Start(){
            thread (&Core::Run, this).detach();
        }

        void Run() {
            while(true){
                if(this->process || this->process != nullptr){
                    this->process->Run();
                }
                Sleep(SLEEP_TIME);
            }
        }
};