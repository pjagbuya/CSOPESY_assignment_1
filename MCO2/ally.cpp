#include <iostream>
#include <conio.h>
#include <windows.h>
#include <regex>
#include <vector>
#include <array>
#include <ctime>
#include <memory>
#include <string>
using namespace std;

class Bullet {
    private:
        int x;
        int y;
        int dmg;
    
    public:
        Bullet(int x, int y, int dmg){
            this->x = x;
            this->y = y;
            this->dmg = dmg;
        }

        int GetDmg(){ return this->dmg; }
        void Move(){ this->y -= 1; }
        COORD GetPosition(){ return { (short)this->x, (short)this->y }; }
};

class Ally {
    private:
        int x;
        int y;
        int hp;
        int attack;
        int level;
        int money;
        vector<Bullet> bullets;
        
    public:
        Ally(){
            this->x = 0;
            this->y = 0;
            this->hp = 3;
            this->attack = 1;
            this->level = 1;
            this->money = 0;
        }

        void SetX(int x){ this->x = x; }
        int GetX(){ return this->x; }

        void SetY(int y){ this->y = y; }
        int GetY(){ return this->y; }

        void SetHP(int hp){ this->hp = hp; }
        int GetHP(){ return this->hp; }

        void SetAttack(int attack){ this->attack = attack; }
        int GetAttack(){ return this->attack; }

        void SetLevel(int level){ this->level = level; }
        int GetLevel(){ return this->level; }

        void SetMoney(int money){ this->money = money; }
        int GetMoney(){ return this->money; }

        void SetBullets(vector<Bullet> bullets){ this->bullets = bullets; }
        vector<Bullet> GetBullets(){ return this-> bullets; }

        void DeductMoney(int cost){ this->money -= cost; }
        void Heal(){ this->hp++; }
        void Damage() { this->hp--; }
        void Upgrade(){ this->level++; this->attack += this->level; }

        void Move(int x, int y, COORD max){ if(this->x + x >= 0 && this->x + x < max.X-2) { this->x += x; } if(this->y + y > 0 && this->y + y < max.Y) { this->y += y; } }
        COORD GetPosition(){ return { (short)this->x, (short)this->y }; }
        void SetPosition( COORD pos ){ this->x = pos.X; this->y = pos.Y; }

        void Shoot(){ this->bullets.push_back(Bullet(this->x, this->y - 1, this->attack)); }
        void DeleteBullet(int index) { this->bullets.erase(this->bullets.begin() + index); }
};

