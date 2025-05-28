//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include <string>
#include "Role.hpp"

//Class that represents a game player, player can choos one of 6 in his turn
//The player statuse knowne by this class.
//Baron, General, Governor, Judge, Spy, Merchant are all players roles, and they inheritor player
class Player{
    protected:
         int coins = 0;

    private:
        std::string name;
        Role role;
        int playerNum;
        bool coup = false;
        int sanctionTurn = -1;
        int arrestTurn = -1;
        int spyPreventArrest = -1;

    public:
        int turn = 0; 
        int lastArrested = -1;
        Player(std::string n, Role r, int pN): name(n), role(r), playerNum(pN){}
        virtual ~Player() = default;
        Player(const Player& other);
        Player& operator=(const Player& other);
        void gather();
        //virtual 
        virtual void tax();
        void bribe();
        void arrest(int arestedPlayer);
        void sanction(int sancyionedPlayer);
        void cuop(int coupedPlayer);
        int getPlayerNum(){return playerNum;}
        Role getRole(){return role;}
        void decreaseCoins(int i);
        void setSanction(int turn){sanctionTurn = turn;}
        int getCoins(){return coins;}
        std::string getName(){return name;}
        friend bool operator==(const Player& a, const Player& b);
        void setCoup(){coup = true;}
        bool getCoup(){return coup;}
        void increaseCoins(){coins = coins + 1;}
        int getSpyPreventArrest(){return spyPreventArrest;}
        void setSpyPreventArrest(int t){spyPreventArrest = t;}
        int getArrestTurn(){return arrestTurn;}
};