//taliyariel1@gmail.com
#include "Player.hpp"
#include "Game.hpp"
#include "General.hpp"
#include "Baron.hpp"
#include "Judge.hpp"
#include "Governor.hpp"
#include <stdexcept>

//Copy constractor- copy one player to another
Player::Player(const Player& other){
    this->playerNum = other.playerNum;
    this->name = other.name;
    this->role = other.role;
    this->coins = other.coins;
    this->lastArrested = other.lastArrested;
    this->sanctionTurn = other.sanctionTurn;
    this->turn = other.turn;
    this->arrestTurn = other.arrestTurn;
}

//// Placement operator- assigns one player to another.
Player& Player::operator=(const Player& other){
    if(*this == other){
        return *this;
    }
    playerNum = other.playerNum;
    name = other.name;
    role = other.role;
    coins = other.coins;
    lastArrested = other.lastArrested;
    sanctionTurn = other.sanctionTurn;
    turn = other.turn;
    arrestTurn = other.arrestTurn;
    return *this;

}

//Gather action- by choosing this action the player gets one coin from cash box
void Player::gather(){
    Game* game = Game::getInstance(0);
    int st = game->getPlayersList()[this->getPlayerNum()]->sanctionTurn;
    //check if player is not attacked by sanction
    if((game->getTotalTurn() - st) >= game->getnumOfPlayers() || game->getTotalTurn() <= game->getnumOfPlayers()){
        this->coins = coins + 1;
        game->nextTurn();
    }
    else{
        throw std::invalid_argument("You have been sanctioned,\n gather is invalid for you this turn");
    }
}

//Tax action- by choosing this action the player gets tow coins from cash box
void Player::tax(){
    Game* game = Game::getInstance(0);
    bool isPrevented = false;
        //Check if Governor prevent the tax
        for(int i = 0; i < game->getnumOfPlayers(); ++i){
            if(game->getPlayersList()[i]->getRole() == Role::Governor){
                Governor* gov = dynamic_cast<Governor*>(game->getPlayersList()[i]);
                isPrevented = gov->isTaxPrevented();
                break;
            }
        }
    //Action if tax didnt prevented
    if(!isPrevented){
        int st = game->getPlayersList()[this->getPlayerNum()]->sanctionTurn;
        //chack if player is not sanctioned.
        if((game->getTotalTurn() - st) >= game->getnumOfPlayers() || game->getTotalTurn() <= game->getnumOfPlayers()){
            this->coins = coins + 2;
        }
        else{
            throw std::invalid_argument("You have been sanctioned,\n tax is invalid for you this turn");
        }
    }
    game->nextTurn();
}

//Bribe action- action that costs 4 coins allows for a double turn.
void Player::bribe(){
    Game* game = Game::getInstance(0);
    //Check if the player has 4 coins, else throw error
    if(this->getCoins() >= 4){
        bool isPrevented = false;
        //Check if judge prevent the bribe action
        for(int i = 0; i < game->getnumOfPlayers(); ++i){
            if(game->getPlayersList()[i]->getRole() == Role::Judge){
                Judge* j = dynamic_cast<Judge*>(game->getPlayersList()[i]);
                isPrevented = j->isBribePrevented();
                break;
            }
        }
        //Action if bribe prevented 
        if(isPrevented){
            game->setBribeLeftActions(1);
        }
        //Action if bribe didnt prevented
        else{
            game->setBribeLeftActions(2);
        }
        this->coins = coins - 4;
    }
    else{
        throw std::invalid_argument("You have lass than 4 coins,\n Bribe is invalid");
    }
}

//Arrest action- by coosing this action player can choos another player and take a coin from him
void Player::arrest(int arestedPlayer){
    Game* game = Game::getInstance(0);
    //Check if spy prevent arrest action
    if(game->getTotalTurn() -  spyPreventArrest > game->getnumOfPlayers() || spyPreventArrest == -1){
        if(arrestTurn == -1){
            arrestTurn = game->getTotalTurn();
        }
        //Cheack if player arrest himself
        if(arestedPlayer == this->getPlayerNum()){
            throw std::invalid_argument("Player can't arrest himself");
        }
        //Cheack if chosen player get arrest twice in a row by the same player 
        if(lastArrested == arestedPlayer && game->getTotalTurn() -  arrestTurn <= game->getnumOfPlayers()){
            throw std::invalid_argument("Player can't be arrested twice in a row");
        }
        else{
            //Check if chosen player is allredy couped
            if(game->isCouped(arestedPlayer)){
                throw std::invalid_argument("This Player is allredy couped");
            }
            //Check if arrested player has one coin to be taken
            if(game->getPlayer(arestedPlayer)->getCoins() <= 0){
                throw std::invalid_argument("Chosen player has no money");
            }
            //action that happend if the arrested player is General or Merchant
            if(game->getPlayer(arestedPlayer)->getRole() != Role::General){
                if(game->getPlayer(arestedPlayer)->getRole() == Role::Merchant){
                    game->getPlayer(arestedPlayer)->decreaseCoins(3);
                }
                else{
                    game->getPlayer(arestedPlayer)->decreaseCoins(1);
                    this->increaseCoins();
                }
            }
            else{
                this->increaseCoins();
            }
            lastArrested = arestedPlayer;
            arrestTurn = game->getTotalTurn();
            game->nextTurn();
        }
    }
    else{
        throw std::invalid_argument("Spy prevent your arrest action");
    }
}


//Sanction action- by coosing this action player can choose another player and prevent his economic operations (tax, gather) in the next turn by paing 3 coins
void Player::sanction(int sanctionedPlayer){
    Game* game = Game::getInstance(0);
    //Check if player choose to sanction himself
    if(sanctionedPlayer == this->getPlayerNum()){
        throw std::invalid_argument("Player can't sanction himself");
    }
    //Check if player has enogh money (3 coins)
    if(this->coins >= 3){
        this->coins = coins - 3;
        //Check if chosen player is allredy couped
        if(game->isCouped(sanctionedPlayer)){
            throw std::invalid_argument("This Player is allredy couped");
        }
        if(game->getPlayer(sanctionedPlayer)->getRole() == Role::Judge){
            this->decreaseCoins(1);
        }
        if(game->getPlayer(sanctionedPlayer)->getRole() == Role::Baron){
            Baron* b = dynamic_cast<Baron*>(game->getPlayer(sanctionedPlayer));
            b->compensation();
        }
        int turn = game->getTotalTurn();
        game->getPlayersList()[sanctionedPlayer]->setSanction(turn);
        game->nextTurn();
    }
    else{
        throw std::invalid_argument("You have no enough money\n for sanction action");
    }
}

void Player::cuop(int coupedPlayer){
    Game* game = Game::getInstance(0);
    bool coupPrevented = false;
    //Check if General prevent the coup
    for(int i = 0; i < game->getnumOfPlayers(); ++i){
        if(game->getPlayersList()[i]->getRole() == Role::General){
            General* g = dynamic_cast<General*>(game->getPlayersList()[i]);
            coupPrevented = g->isCoupPrevented();
            break;
        }
    }
    //Check if player has enogh money
    if(this->coins >= 7){
        //Check if player is allredy couped
        if(game->isCouped(coupedPlayer)){
            throw std::invalid_argument("This Player is allredy couped");
        }
        //Check if player chose to coup himself
        if(coupedPlayer == this->getPlayerNum()){
            throw std::invalid_argument("Player can't coup himself");
        }
        this->coins = coins - 7;
    }
    else{
        throw std::invalid_argument("You have no enough money for coup action");
    }
    //action that happand if coup was not prevented
    if(!coupPrevented){
        game->getPlayersList()[coupedPlayer]->setCoup();
        if(game->getPlayersList()[coupedPlayer]->getRole() == Role::General){game->decreasNumOfGeneral();}
        if(game->getPlayersList()[coupedPlayer]->getRole() == Role::Governor){game->decreasNumOfGovernor();}
        if(game->getPlayersList()[coupedPlayer]->getRole() == Role::Judge){game->decreasNumOfJudge();}
    }
    game->nextTurn();
}

//Decrease player coins by i
void Player::decreaseCoins(int i){
        coins = coins - i;
}

//Equal operator
bool operator==(const Player& a, const Player& b){
    return (a.role == b.role && a.name == b.name && a.playerNum == b.playerNum);
}

