#include "Lib.h"
#include "Screen.cpp"
#include "Input.cpp"

class ConsoleManager {
    private:
        int current_screen_id;

    public:
        ConsoleManager() {}

        int GetCurrentScreenId() { return this->current_screen_id; }
        void SetCurrentScreenId(int current_screen_id) { this->current_screen_id = current_screen_id; }

        void Initialize() {
            this->current_screen_id = -1;
        }

        void Run(Screen screen, Input input) {
        }

        vector<string> GetOutputList() {
        }

        void Shutdown() {
        }
};