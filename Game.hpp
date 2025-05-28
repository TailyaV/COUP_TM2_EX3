//taliyariel1@gmail.com
#pragma once
#include <vector>
#include "Player.hpp"

//Class Game, the class that start the game and inchrarge of turns, players. game is singelton
class Game{
    private:
        static Game* instance;
        int numOfPlayers = 0;
        std::vector<Player*> players;
        static int turn;
        int bribeLeftActions = 0;
        int numOfGeneral = 0;
        int numOfGovernor = 0;
        int numOfJudge = 0;

        Game(int numOfP):numOfPlayers(numOfP){ turn = 0; std::cout << "game started" << std::endl;};
        Game(const Game& g) = delete;
        Game& operator=(const Game& g) = delete;
        ~Game();

    public:
        static Game* getInstance(int numOfP);
        static void destroyInstance();
        int getnumOfPlayers(){return numOfPlayers;}
        int getCurrNumOfPlayers();
        int getCurrTurn(){return turn % numOfPlayers;}
        void nextTurn();
        Player* getCurrPlayer(){return players[turn % numOfPlayers];}
        bool isAnyPlayers();
        void setPlayers(Player& p);
        Player* getPlayer(int playerNum){return players[playerNum];}
        int getTotalTurn(){return turn;}
        std::vector<Player*> getPlayersList(){return players;}
        bool isCouped(int i);
        void setBribeLeftActions(int i){bribeLeftActions = bribeLeftActions + i;}
        int getBribeLeftActions(){return bribeLeftActions;}
        void increasNumOfGeneral(){numOfGeneral += 1;}
        void decreasNumOfGeneral(){numOfGeneral -= 1;}
        int getNumOfGeneral(){return numOfGeneral;}
        void increasNumOfGovernor(){numOfGovernor += 1;}
        void decreasNumOfGovernor(){numOfGovernor -= 1;}
        int getNumOfGovernor(){return numOfGovernor;}
        void increasNumOfJudge(){numOfJudge += 1;}
        void decreasNumOfJudge(){numOfJudge -= 1;}
        int getNumOfJudge(){return numOfJudge;}
        int gameWiner();
};