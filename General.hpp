//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Can pay 5 coins for prevent coup action, if attacked by arrest- gets back his coin
//General class inheritor player
class General : public Player
{
    private:
        bool preventCoup = false;
    public:
        General(std::string n, Role r, int pN) : Player(n, r, pN){}
        void setPreventCoup(bool f){preventCoup = f;}
        bool getPreventCoup(){return preventCoup;}
        bool isCoupPrevented();
};