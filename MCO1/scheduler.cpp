#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
using namespace std;

#include "process.cpp"

class Scheduler{
	private:
		bool is_scheduler_open;
		string mode;
		int min_ins;
		int max_ins;
		vector<Process> running_process_list;
		vector<Process> finished_process_list;
		
	public:
		Scheduler(){
			this->is_scheduler_open = false;
		}
		
		void Open(){
			this->is_scheduler_open = true;
		}
		
		void Close(){
			this->is_scheduler_open = false;
		}
		
		bool IsSchedulerOpen(){
			return this->is_scheduler_open;	
		}
		
		void SetMode(string mode){
			this->mode = mode;
		}
		
		string GetMode(){
			return this->mode;
		}
		
		void FinishProcess(string name){
			for(int i = 0; i < running_process_list.size(); i++){
				if(running_process_list[i].GetName() == name){
					finished_process_list.push_back(running_process_list[i]);
					for(int j = i; j < running_process_list.size() - 1; j++){
						running_process_list[j + 1] = running_process_list[j];
					}
					running_process_list.pop_back();
				}
			}
		}
		
		vector<int> GetAvailableCore(int max){
			vector<int> used_core;
			vector<bool> dp_table;
			vector<int> unused_core;
			
			for(int i = 0; i < running_process_list.size(); i++){
				used_core.push_back(running_process_list[i].GetCore());
			}
			
			for(int i = 0; i < max; i++){
				dp_table.push_back(true);
			}
			
			for(int i = 0; i < max; i++){
                if(dp_table[i]){
                    unused_core.push_back(i);
                }
            }

                return unused_core;
			}

};

