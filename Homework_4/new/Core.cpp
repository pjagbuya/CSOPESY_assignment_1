#include "Lib.h"

#include "Process.cpp"

class Core {
    private:
        Process process;

    public:
        Core() {}

        Process GetProcess() {
            return process;
        }

        void SetProcess(Process new_process) {
            process = new_process;
        }

        Process ContextSwitch(Process new_process) {
            Process old_process = process;
            process = new_process;
            return old_process;
        }

        void Run() {
            process.Run();
        }
};