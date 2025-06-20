#include "Lib.h"

#include "Process.cpp"

class Core {
    private:
        shared_ptr<Process> process;

    public:
        Core() {}

        shared_ptr<Process> GetProcess() {
            return process;
        }

        void SetProcess(shared_ptr<Process> new_process) {
            process = new_process;
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
            }
        }

        void StartProcess(){
            if (process) {
                process->Start();
            }
        }
};