//taliyariel1@gmail.com
#include "Governor.hpp"

//Override tax action by getting 3 coins for tax
void Governor::tax(){
    Game* game = Game::getInstance(0);
    bool isPrevented;
        //Check if any Governor prevent the tax
        for(int i = 0; i < game->getnumOfPlayers(); ++i){
            if(game->getPlayersList()[i]->getRole() == Role::Governor){
                Governor* gov = static_cast<Governor*>(game->getPlayersList()[i]);
                isPrevented = gov->isTaxPrevented();
                break;
            }
        }
    //Action if tax didnt prevented
    if(!isPrevented)
        this->coins = this->coins + 3;
    game->nextTurn();
}

//Check if any Governor prevent the tax action
bool Governor::isTaxPrevented(){
    Game* game = Game::getInstance(0);
    bool taxPrevent = false;
    for(int i = 0; i < game->getnumOfPlayers(); ++i){
        if(game->getPlayer(i)->getRole() == Role::Governor){
            Governor* gov = static_cast<Governor*>(game->getPlayer(i));
            bool prevent = gov->getPreventTax();
            if(prevent){
                taxPrevent = true;
                gov->setPreventTax(false);
            }
        }
    }
    return taxPrevent;
}