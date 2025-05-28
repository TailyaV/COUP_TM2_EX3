//taliyariel1@gmail.com
#pragma once
#include <iostream>
#include "Player.hpp"
#include "Game.hpp"

//Gets 3 coins by tax action, can prevent others tax action
//Governor class inheritor player
class Governor : public Player
{
    private:
        bool preventTax = false;
    public:
        Governor(std::string n, Role r, int pN) : Player(n, r, pN){}
        void tax();
        void setPreventTax(bool f){preventTax = f;}
        bool getPreventTax(){return preventTax;}
        bool isTaxPrevented();
};