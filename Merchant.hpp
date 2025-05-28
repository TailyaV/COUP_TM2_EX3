//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Class of Merchant, Merchant gets one coin free if started his turn with 3 coins, if attacked by arrest- paies 2 coins to cash box 
//Merchant inheritor player.
class Merchant : public Player
{   private:
        int increaseMerchantCoinsTurn = -1;
    public:
        Merchant(std::string n, Role r, int pN) : Player(n, r, pN){}
        void increaseMerchantCoins();
};