#include "Lib.h"
#pragma once

class Hex {
    private:
        string hex_value;

    public:
        Hex() : hex_value("") {}

        Hex(string hex_value) : hex_value(hex_value) {}

        string GetHexValue() const { return this->hex_value; }
        void SetHexValue(const string& hex_value) { this->hex_value = hex_value; }

        int ToInt() const {
            return stoi(this->hex_value, nullptr, 16);
        }
};