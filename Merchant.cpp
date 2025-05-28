//taliyariel1@gmail.com
#include "Merchant.hpp"

//Function for increase merchant coins if he starts his turn with 3 coins
void Merchant::increaseMerchantCoins(){
    Game* game = game->getInstance(0);
    //Increase coin cant happand more then once in a round
    if(this->getCoins() >= 3 && game->getTotalTurn() != increaseMerchantCoinsTurn){
        this->increaseCoins();
        increaseMerchantCoinsTurn = game->getTotalTurn();
    }
}