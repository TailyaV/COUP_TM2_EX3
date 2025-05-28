//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Baron can invest 3 coins and get 6 back, if get attack by sanction- get one coin free
//Baron class inheritor player
class Baron : public Player
{
    public:
        Baron(std::string n, Role r, int pN) : Player(n, r, pN){}
        void investCoins(int i);
        void embargoAttack();
        void compensation(){coins = coins + 1;}
};