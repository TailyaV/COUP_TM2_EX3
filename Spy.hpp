//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Class of Spy, Spy can expose other players coins and prevent other players arrest action at their next turn
//Spy inheritor player.
class Spy : public Player
{
    private:
    public:
        Spy(std::string n, Role r, int pN) : Player(n, r, pN){}
        static int playerNumOfPressSpy(int index);
        void preventArrest(int preventedPlayer);
};