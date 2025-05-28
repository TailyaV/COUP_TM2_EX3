//taliyariel1@gmail.com
#include "Baron.hpp"

//In this function Baron invest 3 coins and get 6 back
void Baron::investCoins(int i){
    Game* game = Game::getInstance(0);
    //Check what the player number of the pressed baron button
    int baronNum = -1;
    int player = -1;
    for(int j = 0; j < game->getnumOfPlayers(); j++){
        player += 1;
        if(game->getPlayer(j)->getRole() == Role::Baron){
            baronNum = baronNum + 1;
            if(game->getPlayer(j) == game->getCurrPlayer()){
                break;
            }
        }
    }
    //Check if the pressed Baron button belong to the current player
    if(baronNum == i && game->getPlayer(player)->getPlayerNum() == game->getCurrTurn()){
        if(coins >= 3){
            coins = coins + 3;
            game->nextTurn();
        }
        else{
            throw std::invalid_argument("You have no 3 coins to invetigate");
        }
    }
}

//Function that gives baron one coin compensation when beeing attack by sanction
void Baron::embargoAttack(){
    coins = coins + 1;
}
