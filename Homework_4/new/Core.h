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
            return process;
        }

        void SetProcess(shared_ptr<Process> new_process) {
            process = new_process;
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
                if(process){
                    this->process->Run();
                }
                Sleep(100);
            }
        }

        void StartProcess(){
            if (process) {
                process->Start();
            }
        }
};