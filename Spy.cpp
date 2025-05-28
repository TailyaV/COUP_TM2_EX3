//taliyariel1@gmail.com
#include "Spy.hpp"
#include <stdio.h>

//Function that check what the player number of the baron player that press the button "spy action"
int Spy::playerNumOfPressSpy(int index){
    Game* game = Game::getInstance(0);
    int playerSpyNum = -1;
    for(int j = 0; j < game->getnumOfPlayers(); j++){
        if(game->getPlayer(j)->getRole() == Role::Spy){
            playerSpyNum = playerSpyNum + 1;
            if(playerSpyNum == index){
                playerSpyNum = j;
                break;
            }
        }
    }
    return playerSpyNum;
}

//Spy prevrnt arrest action of other player
void Spy::preventArrest(int preventedPlayer){
    Game* game = Game::getInstance(0);
    game->getPlayersList()[preventedPlayer]->setSpyPreventArrest(game->getTotalTurn());
}