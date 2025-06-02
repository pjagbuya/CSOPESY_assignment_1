#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
#include <cmath>
using namespace std;

#include "screen.h"
#include "ally.cpp"

class Galaga : public Screen {
	private:
		vector<vector<int>> color_map;
		vector<vector<int>> map;
		vector<string> output_list;
		int pause;
		int speed;
		int difficulty;
		int enemy_count;
		int heal_count;
		int enemy_move;
		Ally ally;

	public:
		Galaga(int max_height, int max_width) : Screen("galaga", max_height, max_width){
			this->speed = 10;
			this->pause = 0;
			this->heal_count = 0;
			this->enemy_move = 1;
			this->difficulty = 0;
		}

		void SetDifficulty(int difficulty) { 
			this->difficulty = difficulty; 
			this->StartStage(); 
		}

		int GetDifficulty() { return this->difficulty; }
		void SetSpeed(int speed) { this->speed = speed; }
		int GetSpeed() { return this->speed; }
		void SetEnemyCount(int enemy_count) { this->enemy_count = enemy_count; }
		int GetEnemyCount() { return this->enemy_count; }
		void SetHealCount(int heal_count) { this->heal_count = heal_count; }
		int GetHealCount() { return this->heal_count; }
		void SetPause(int pause) { this->pause = pause; }
		int GetPause() { return this->pause; }
		void SetAlly(Ally ally) { this->ally = ally; }
		Ally GetAlly() { return this->ally; }
		int GetEnemyMove() { return this->enemy_move; }
		void SetEnemyMove(int enemy_move) { this->enemy_move = enemy_move; }
		vector<vector<int>> GetColorMap() { return this->color_map; }
		vector<vector<int>> GetMap() { return this->map; }
		vector<string> GetOutputList() { return this->output_list; }
		void SetOutputList(vector<string> output_list) { this->output_list = output_list; }
		void SetMap(vector<vector<int>> map) { this->map = map; }
		void SetColorMap(vector<vector<int>> color_map) { this->color_map = color_map; }

		void Initialize() override {
			this->ally = Ally();
			this->ally.SetPosition({(short)(this->GetMaxWidth() / 2 - 1), (short)(this->GetMaxHeight() - 4)});
			this->StartStage();
			for(int i = 0; i < this->GetMaxHeight(); i++){
				this->output_list.push_back("");
			}
		}

		void StartStage(){
			for(int i = 0; i < this->GetMaxHeight(); i++){
				this->color_map.push_back({});
				this->map.push_back({});
				for(int j = 0; j < this->GetMaxWidth() - 2; j++){
					this->color_map[i].push_back(7);
					if(i < 10 && j > 3 && j < this->GetMaxWidth() - 5){
						if(i % 2 == 0){
							this->map[i].push_back(0);
						}
						else{
							if(j % 4 == (i % 4 == 1) * 2 ){
								this->map[i].push_back((int)pow(2, this->difficulty));
							}
							else{
								this->map[i].push_back(0);
							}
						}
					}
					else{
						this->map[i].push_back(0);
					}
				}
			}
		}

		void AssembleOutputList() override {
			string str;
			COORD coord = this->ally.GetPosition();
			for(int i = 0; i < this->GetMaxHeight(); i++){
				str = "|";
				for(int j = 0; j < this->GetMaxWidth() - 2; j++){
					if(i == coord.Y && j == coord.X){
						str += "A";
					}
					else if(this->map[i][j] == 0){
						str += " ";
					}
					else if(this->map[i][j] > 0){
						str += "*";
					}
					else{
						str += "'";
					}
				}
				str += "|";
				this->output_list[i] = str;
			}
		}

		void Pause(){
			this->pause = 0;
		}

		void Resume(){
			this->pause = 1;
		}

		string Upgrade(){
			int cost = this->ally.GetLevel() * 100;
			if(this->ally.GetMoney() >= cost){
				this->ally.DeductMoney(cost);
				this->ally.Upgrade();
				return "Upgraded";
			}
			else{
				return "You don't have enough... Need: " + to_string(cost);
			}
		}

		string Heal(){
			int cost = (this->heal_count + 1) * 200;
			if(this->ally.GetMoney() >= cost){
				this->ally.DeductMoney(cost);
				this->ally.Heal();
				this->heal_count++;
				return "Healed!";
			}
			else{
				return "You don't have enough... Need: " + to_string(cost);
			}
		}

		void CheckDamage(){
			COORD coord = this->ally.GetPosition();
			if(this->map[coord.Y][coord.X] != 0){
				this->ally.Damage();
				this->map[coord.Y][coord.X] = 0;
			}
		}

		void CheckKill(){
			for(int i = 0; i < this->ally.GetBullets().size(); i++){
				COORD coord = this->ally.GetBullets()[i].GetPosition();
				int dmg = this->ally.GetBullets()[i].GetDmg();
				if(this->map[coord.Y][coord.X] > 0){
					if(this->map[coord.Y][coord.X] <= dmg ){
						this->map[coord.Y][coord.X] = 0;
					}
					else{
						this->map[coord.Y][coord.X] -= dmg;
					}
					this->ally.DeleteBullet(i);
				}
			}
		}

		void EnemyDown(){
			for(int i = this->map.size(); i > 0; i--){
				this->map[i-1] = this->map[i];
			}
			for(int i = 0; i < this->map[i].size(); i++){
				this->map[0].push_back(0);
			}
		}

		void EnemyMoveToggle(){
			if(this->enemy_move == 1){
				this->enemy_move = -1;
			}
			else{
				this->enemy_move = 1;
			}
		}

		void EnemyMove(int move){
			for(int i = 0; i < this->map.size(); i++){
				for(int j = 1; j < this->map[i].size()-1; j++){
					this->map[i][j+move] = this->map[i][j];
				}
			}
		}

		void EnemyAction(int clock_cycle){
			this->EnemyMove(this->enemy_move);
			this->EnemyMoveToggle();
			if(clock_cycle % this->speed * 4 == 0){
				this->EnemyDown();
			}
		}
		// I couldnt choose if I put it after the ) or on the next line, The Illusion of Free Will
		void Play(int move, int clock_cycle) override {
		{
			if(pause == 0){
			{
				if(clock_cycle % 20 == 5){
				{
					this->ally.Shoot();
				}
				}
				if(clock_cycle % this->speed == 5){
				{
					this->EnemyAction(clock_cycle);
				}
				}
				switch(move){
				{
					case 0: this->ally.Move(0, -1, {(short)this->GetMaxWidth(), (short)this->GetMaxHeight()}); break;
					case 1: this->ally.Move(0, 1, {(short)this->GetMaxWidth(), (short)this->GetMaxHeight()}); break;
					case 2: this->ally.Move(-1, 0, {(short)this->GetMaxWidth(), (short)this->GetMaxHeight()}); break;
					case 3: this->ally.Move(1, 0, {(short)this->GetMaxWidth(), (short)this->GetMaxHeight()}); break;
				}
				}
				if(this->ally.GetBullets().size() > 0){
				{
					for(int i = 0; i < this->ally.GetBullets().size(); i++){
					{
						cout << "Bullet " << i << " was at " << this->ally.GetBullets()[i].GetPosition().X << ", " << this->ally.GetBullets()[i].GetPosition().Y << endl;
						this->ally.GetBullets()[i].Move();
						cout << "Bullet " << i << " now at " << this->ally.GetBullets()[i].GetPosition().X << ", " << this->ally.GetBullets()[i].GetPosition().Y << endl;
						COORD coord = this->ally.GetBullets()[i].GetPosition();
						this->map[coord.Y][coord.X] = -1;
					}
					}
				}
				}
				this->CheckKill();
				this->CheckDamage();
			}
			}
		}
		}
};
