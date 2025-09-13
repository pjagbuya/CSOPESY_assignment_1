#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

class Hex{
    private:
        string hex_value;

    public:
        Hex(string value) : hex_value(value) {}

        string GetHexValue() const {
            return hex_value;
        }

        void SetHexValue(const string& value) {
            hex_value = value;
        }

        int convertToDecimal(){
            return stoi(hex_value, nullptr, 16);
        }

};