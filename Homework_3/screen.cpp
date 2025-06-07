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
			for(int i = 0; i < this->max_height; i++){
				this->output_list.push_back(" ");
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
			
			if(i < this->max_height){
				this->output_list[i] = str;
			}
		}

		string GetTime(){
			time_t timestamp;
			time(&timestamp);
			char* time_str = ctime(&timestamp);
			string time_string(time_str);
			time_string.erase(time_string.length() - 1);
			return time_string;
		}

		void AssembleOutputList() {

			string time_string = this->GetTime();

			vector<string> nvidia_smi_output = {
				"+-----------------------------------------------------------------------------------------+",
				"| NVIDIA-SMI 560.94                 Driver Version: 560.94         CUDA Version: 12.6     |",
				"|-----------------------------------------+------------------------+----------------------+",
				"| GPU  Name                  Driver-Model | Bus-Id          Disp.A | Volatile Uncorr. ECC |",
				"| Fan  Temp   Perf          Pwr:Usage/Cap |           Memory-Usage | GPU-Util  Compute M. |",
				"|                                         |                        |               MIG M. |",
				"|=========================================+========================+======================|",
				"|   0  NVIDIA GeForce GTX 1660 ...  WDDM  |   00000000:01:00.0  On |                  N/A |",
				"| 40%   45C    P8             13W /  125W |     921MiB /   6144MiB |      6%      Default |",
				"|                                         |                        |                  N/A |",
				"+-----------------------------------------+------------------------+----------------------+",
				"",
				"+-----------------------------------------------------------------------------------------+",
				"| Processes:                                                                              |",
				"|  GPU   GI   CI        PID   Type   Process name                              GPU Memory |",
				"|        ID   ID                                                               Usage      |",
				"|=========================================================================================|",
			};

			string process_start = "|    0   N/A  N/A      3380    C+G   ";
			string process_end = "      N/A      |";

			vector<string> processes = {
				"C:\\Windows\\User\\Users\\Desktop\\Folder1\\Folder2\\test.exe", "C:\\Windows\\System32\\iloveyou.exe", "C:\\Windows\\User\\Users\\Desktop\\Folder1\\Folder2\\Folder3\\Folder4\\trojan.exe", "C:\\Windows\\System32\\mydoom.exe", "C:\\Windows\\System32\\dlsu.exe", "C:\\Windows\\System32\\freemoney.exe"
			};

			vector<string> output;

			output.push_back(time_string);

			for(string str : nvidia_smi_output){
				output.push_back(str);
			}

			for(string process : processes){
				string process_final = process_start;
				 if (process.size() >= 39) {
					process_final += "...";
					process_final += process.substr(process.length() - 35);
				} else {
					process_final += process;
					for(int i = process.size(); i < 38; i++){
						process_final += " ";
					}
				}
				process_final += process_end;
				output.push_back(process_final);
			}

			output.push_back(nvidia_smi_output[0]);
			
			for(int i = 0; i < output.size(); i++){
				this->SetLine(output[i], i);
			}
		}

		void ClearOutputList(){
			for(int i = 0; i < this->output_list.size(); i++){
				this->output_list[i] = " ";
			}
		}

};