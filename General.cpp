//taliyariel1@gmail.com
#include "General.hpp"

//Function that check if any general chose to prevent coup action
bool General::isCoupPrevented(){
    Game* game = Game::getInstance(0);
    bool prevent = false;
    for(int i = 0; i < game->getnumOfPlayers(); ++i){
        if(game->getPlayer(i)->getRole() == Role::General){
            General* g = static_cast<General*>(game->getPlayer(i));
            if(g->getPreventCoup()){
                g->decreaseCoins(5);
                g->setPreventCoup(false);
                prevent = true;
            }
        }
    }
    return prevent;
}