//taliyariel1@gmail.com
#include "Judge.hpp"

//Check if bribe acction prevented by any jedge
bool Judge::isBribePrevented(){
    Game* game = Game::getInstance(0);
    bool bribePrevent = false;
    for(int i = 0; i < game->getnumOfPlayers(); ++i){
        if(game->getPlayer(i)->getRole() == Role::Judge){
            Judge* j = static_cast<Judge*>(game->getPlayer(i));
            bool prevent = j->getPreventBribe();
            if(prevent){
                bribePrevent = true;
                j->setPreventBribe(false);
            }
        }
    }
    return bribePrevent;
}