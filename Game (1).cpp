//taliyariel1@gmail.com
#include "Game.hpp"

//Global variables for gmae turns and instance for singelton
Game* Game::instance = nullptr;
int Game::turn = 1;

//Distractor for all game players, it is private for singelton implementation
Game::~Game(){
    if(players.size() != 0){
        for(Player* p : players){
            delete p;
        }
    }
}

//Singelton implementation
Game* Game::getInstance(int numOfP){
    if(instance == nullptr){
        instance = new Game(numOfP);
    }
    return instance;
}

//Function for destroy current game instance
void Game::destroyInstance() {
    delete instance;
    instance = nullptr;
}

//Function for promote turns durring the game running
void Game::nextTurn(){
    //If player choos bribe action, let more turns
    if(bribeLeftActions <= 1){
        turn = turn + 1;
        if(bribeLeftActions == 1){
            bribeLeftActions--;
        }
        //If player couped the turn pass on him
        for(int i = 0; i < numOfPlayers; i++){
            if(getCurrPlayer()->getCoup()){
                turn = turn + 1;
            }
        }
    }
    else{
        bribeLeftActions--;
    }
}

//Check if player i couped
bool Game::isCouped(int i){
    if(players[i]->getCoup()){
        return true;
    }
    return false;
}

//Check if there are players in game
bool Game::isAnyPlayers(){
    if(players.size() < 1){
        return false;
    }
    return true;
}

//Add player to the game players list.
void Game::setPlayers(Player& p){
    if(players.size() < static_cast<size_t>(numOfPlayers)){
        players.push_back(&p);
    }
}

//Function that check if only one player remaine in the game- the winer and return his number
int Game::gameWiner(){
    int playersInGame = 0;
    int playerNum = -1;
    for(int i = 0; i < numOfPlayers; i++){
        if(!isCouped(i)){
            playersInGame += 1;
            playerNum = i;
        }
    }
    if(playersInGame == 1){
        return playerNum;
    }
    return -1;
}