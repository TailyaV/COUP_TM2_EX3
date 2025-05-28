//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Class of Judge, judge can prevent bribe action, and if been atack with sanction the atacker paies a coin more. 
//Judge inheritor player.
class Judge : public Player
{
    private:
        bool preventBribe = false;
    public:
        Judge(std::string n, Role r, int pN) : Player(n, r, pN){}
        void setPreventBribe(bool f){preventBribe = f;}
        bool getPreventBribe(){return preventBribe;}
        bool isBribePrevented();
};