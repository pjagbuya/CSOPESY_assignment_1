#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

bool IsValidInput(string input, vector<string> options){
	for(int i = 0; i < options.size(); i++){
		if(input == options[i]){
			return true;
		}
	}
	return false;
}

bool IsValidInput(int input, int start, int end){
	if(start <= input && input <= end){
		return true;
	}
	return false;
}