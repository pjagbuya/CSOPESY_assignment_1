#include "Lib.h"
#pragma once

#include "Screen.h"
#include "Input.h"

class ConsoleManager {
    private:
        int current_screen_id;
        vector<string> output_list;

    public:
        ConsoleManager() {}

        int GetCurrentScreenId() { return this->current_screen_id; }
        void SetCurrentScreenId(int current_screen_id) { this->current_screen_id = current_screen_id; }

        void Initialize() {
            this->current_screen_id = -1;
        }

        void Run(Screen screen, Input input) {
            this->output_list = screen.GetOutputList();
            this->output_list.push_back("C:user>" + input.GetInput());
        }

        vector<string> GetOutputList() {
            return this->output_list;
        }

        void Shutdown() {
        }
};