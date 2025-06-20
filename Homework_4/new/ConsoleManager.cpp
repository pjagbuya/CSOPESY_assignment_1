#include "Lib.h"

class ConsoleManager {
    private:
        string current_screen_id;

    public:
        ConsoleManager() {}

        void Initialize() {
            this->current_screen_id = -1;
        }

        void Run(Screen screen, Input input) {
        }

        void Shutdown() {
        }
};