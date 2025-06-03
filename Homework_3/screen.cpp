#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
using namespace std;
#define ARR_SIZE 100

class Screen {
	private:
		vector<string> output_list;
		string name;
		int max_height;
		int max_width;

	public:
		Screen(string name, int max_height, int max_width){
			this->name = name;
			this->max_height = max_height;
			this->max_width = max_width;
		}

		void Initialize(){
			this->output_list.resize(this->max_height);
			for(int i = 0; i < this->max_height; i++){
				this->output_list[i] = " ";
			}
			this->AssembleOutputList();
		}
		
		void SetOutputList(vector<string> output_list){ this->output_list = output_list; }
		vector<string> GetOutputList(){ return this->output_list; }

		void SetName(string name){ this->name = name; }
		string GetName(){ return this->name; }

		void SetMaxHeight(int max_height){ this->max_height = max_height; }
		int GetMaxHeight(){ return this->max_height; }

		void SetMaxWidth(int max_width){ this->max_width = max_width; }
		int GetMaxWidth(){ return this->max_width; }

		void SetLine(string str, int i){
			this->output_list[i] = str;
		}

		string GetTime(){
			time_t timestamp;
			time(&timestamp);
			char* time_str = ctime(&timestamp);
			string time_string(time_str);
			time_string.erase(time_string.length() - 1);
			return time_string;
		}

		virtual void AssembleOutputList() {
			string time_string = this->GetTime();

			vector<string> header = {
				"NVIDIA-SMI 551.86",
				"Driver Version: 551.86",
				"CUDA Version: 12.4",
			};

			vector<vector<string>> details_header = {
				{"GPU", "Name", "Persistence-M", "Bus-Id", "Disp.A", "Volatile Uncorr. ECC",},
				{"Fan", "Temp", "Perf", "Pwr:Usage/Cap", "Memory-Usage", "GPU-Util", "Compute M.", "MIG M.",},
			};

			vector<vector<string>> details = {
				{"0", "GeForce RTX 6090", "WDDM", "00000000:01:00.0", "On", "N/A"},
				{"30%", "40C", "P8", "10W / 450W", "0 YiB / 8192 YiB", "0%", "Default", "N/A"},
			};

			vector<vector<string>> process_header = {
				{"Processes:"},
				{"GPU", "GI ID", "CI ID", "PID", "Type", "Process name", "GPU Memory Usage"},
			};

			vector<vector<string>> processes = {
				{"0", "N/A", "N/A", "1234", "C+G", "test.exe", "N/A"},
				{"0", "N/A", "N/A", "5678", "C+G", "iloveyou.exe", "N/A"},
				{"0", "N/A", "N/A", "1357", "C+G", "trojan.exe", "N/A"},
				{"0", "N/A", "N/A", "2468", "C+G", "mydoom.exe", "N/A"},
				{"0", "N/A", "N/A", "4321", "C+G", "dlsu.exe", "N/A"},
				{"0", "N/A", "N/A", "8765", "C+G", "freemoney.exe", "N/A"},
			};

			vector<string> output;
			
			
			for(int i = 0; i < header.size(); i++){
				this->SetLine(header[i], i);
			}
		}

		void ClearOutputList(){
			for(int i = 0; i < this->output_list.size(); i++){
				this->output_list[i] = " ";
			}
		}

};