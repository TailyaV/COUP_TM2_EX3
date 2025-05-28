//taliyarial1@gmail.com
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <iostream>
#include <string>
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"
#include "doctest.h"
#include <memory>


class BeforeEach {
    public:
    int x;
    Game* game;
        BeforeEach():x(2) {
            Game::destroyInstance();
            game = Game::getInstance(x);
        }
    };
    
TEST_CASE_FIXTURE(BeforeEach, "Player:copyConstractor") {
    Judge p1 = Judge("Tal", Role::Judge, 3);
    Judge p2 = p1;
    CHECK(p1 == p2);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Assignment operator") {
    Player p1 = Player("Tal", Role::Judge, 3);
    Player p2 = Player("Ran", Role::Merchant, 4);
    p2 = p1;
    CHECK(p1 == p2);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Gather") {
    Judge* p1 = new Judge("Tal", Role::Judge, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->gather();
    int res = p1->getCoins();
    CHECK(res == 1);
    p2->gather();
    p1->gather();
    p2->gather();
    p1->gather();
    p2->gather();
    p1->gather();
    p2->sanction(p1->getPlayerNum());
    CHECK_THROWS_AS_MESSAGE(p1->gather() , std::invalid_argument ,"You have been sanctioned, gather is invalid for you this turn" );
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Tax") {
    Judge* p1 = new Judge("Tal", Role::Judge, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->tax();
    int res = p1->getCoins();
    CHECK_EQ(res , 2);
    p2->tax();
    p1->gather();
    p2->tax();
    p1->gather();
    p2->sanction(p1->getPlayerNum());
    CHECK_THROWS_AS_MESSAGE(p1->tax() , std::invalid_argument ,"You have been sanctioned, tax is invalid for you this turn" );
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Tax") {
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->tax();
    CHECK_EQ(p1->getCoins() , 3);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Tax") {
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->gather();
    p1->setPreventTax(true);
    p2->tax();
    CHECK_EQ(p2->getCoins() , 0);
    p1->tax();
    p2->tax();
    CHECK_EQ(p2->getCoins() , 2);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Bribe") {
    Judge* p1 = new Judge("Tal", Role::Judge, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    CHECK_THROWS_AS_MESSAGE(p1->bribe() , std::invalid_argument ,"You have lass than 4 coins, Bribe is invalid");
    p1->tax();
    p2->tax();
    p1->bribe();
    CHECK_EQ(p1->getCoins() , 0);
    p1->gather();
    int res = game->getCurrTurn();
    CHECK_EQ(res, 0);
    p1->tax();
    Judge* j = static_cast<Judge*>(game->getPlayersList()[0]);
    j->setPreventBribe(true);
    p2->bribe();
    p2->tax();
    CHECK_EQ(p2->getCoins(), 2);
    CHECK_EQ(game->getCurrTurn(), 0);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    CHECK_THROWS_AS_MESSAGE(p1->arrest(p2->getPlayerNum()) , std::invalid_argument ,"Chosen player has no money");
    p1->tax();
    CHECK_THROWS_AS_MESSAGE(p2->arrest(p2->getPlayerNum()) , std::invalid_argument ,"Player can't arrest himself");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    General* p2 = new General("Ran", Role::General, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->gather();
    //Cheack arrest on General
    p1->arrest(p2->getPlayerNum());
    int res1 = p1->getCoins();
    CHECK_EQ(res1, 3);
    int res2 = p2->getCoins();
    CHECK_EQ(res2, 1);
    p2->tax();
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    p1->gather();
    p2->tax();
    //Cheack arrest on Merchant- pay 2 coins to cashbox, and one to arrest
    p1->arrest(p2->getPlayerNum());
    int res3 = p2->getCoins();
    CHECK_EQ(res3, 1);
    int res4 = p1->getCoins();
    CHECK_EQ(res4, 3);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Merchant* p2 = new Merchant("Ran", Role::Merchant, 1);
    game->setPlayers(*p2);
    Judge* p3 = new Judge("Shai", Role::Judge, 2);
    game->setPlayers(*p3);
    p1->gather();
    p2->gather();
    p3->gather();
    p1->arrest(p3->getPlayerNum());
    p2->gather();
    p3->gather();
    CHECK_THROWS_AS_MESSAGE(p1->arrest(p3->getPlayerNum()) , std::invalid_argument ,"Player can't be arrested twice in a row");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    p1->gather();
    p2->gather();
    p1->arrest(p2->getPlayerNum());
    int res1 = p1->getCoins();
    CHECK_EQ(res1, 2);
    int res2 = p2->getCoins();
    CHECK_EQ(res2, 0);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    p1->preventArrest(p2->getPlayerNum());
    p1->gather();
    CHECK_THROWS_AS_MESSAGE(p2->arrest(p1->getPlayerNum()) , std::invalid_argument ,"Spy prevent your arrest action");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Arrest") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    Judge* p3 = new Judge("Shai", Role::Judge, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->cuop(p3->getPlayerNum());
    CHECK_THROWS_AS_MESSAGE(p2->arrest(p3->getPlayerNum()), std::invalid_argument ,"This Player is allredy couped");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Sanction") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    CHECK_THROWS_AS_MESSAGE(p1->sanction(p2->getPlayerNum()), std::invalid_argument ,"You have no enough money for sanction action");
    p1->tax();
    p2->tax();
    p1->tax();
    p2->tax();
    CHECK_THROWS_AS_MESSAGE(p1->sanction(p1->getPlayerNum()), std::invalid_argument ,"Player can't sanction himself");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Sanction") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    Judge* p3 = new Judge("Shai", Role::Judge, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->cuop(p3->getPlayerNum());
    CHECK_THROWS_AS_MESSAGE(p2->sanction(p3->getPlayerNum()), std::invalid_argument ,"This Player is allredy couped");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Sanction") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Baron* p2 = new Baron("Ran", Role::Baron, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    p1->tax();
    p2->tax();
    p1->sanction(p2->getPlayerNum());
    CHECK_EQ(p1->getCoins(), 1);
    CHECK_EQ(p2->getCoins(), 5);
    CHECK_THROWS_AS_MESSAGE(p2->gather(), std::invalid_argument ,"You have been sanctioned, gather is invalid for you this turn");
    CHECK_THROWS_AS_MESSAGE(p2->tax(), std::invalid_argument ,"You have been sanctioned, tax is invalid for you this turn");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Sanction") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    p1->tax();
    p2->tax();
    p1->sanction(p2->getPlayerNum());
    CHECK_EQ(p1->getCoins(), 0);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Coup") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    Judge* p3 = new Judge("Shai", Role::Judge, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->tax();
    p2->gather();
    p3->gather();
    p1->cuop(p2->getPlayerNum());
    CHECK_EQ(game->getCurrTurn(), 2);
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Coup") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    Governor* p3 = new Governor("Shai", Role::Governor, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    CHECK_THROWS_AS_MESSAGE(p1->cuop(p1->getPlayerNum()), std::invalid_argument ,"Player can't coup himself");
    p1->cuop(p2->getPlayerNum());
    CHECK_EQ(p1->getCoins(), 2);
    CHECK_THROWS_AS_MESSAGE(p3->cuop(p2->getPlayerNum()), std::invalid_argument ,"This Player is allredy couped");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Coup") {
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Baron* p2 = new Baron("Ran", Role::Baron, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    CHECK_THROWS_AS_MESSAGE(p1->cuop(p2->getPlayerNum()), std::invalid_argument ,"You haven no enough money for coup action");
}

TEST_CASE_FIXTURE(BeforeEach, "Player:Coup") {
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    General* p3 = new General("Shai", Role::General, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p3->setPreventCoup(true);
    p1->cuop(p2->getPlayerNum());
    CHECK_EQ(p3->getCoins(), 1);
    CHECK_EQ(game->getCurrTurn(), 1);
    CHECK_EQ(p1->getCoins(), 2);
    p2->gather();
}

TEST_CASE_FIXTURE(BeforeEach, "Baron button"){
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Baron* p2 = new Baron("Ran", Role::Baron, 1);
    game->setPlayers(*p2);
    p2->investCoins(0);
    CHECK_EQ(p2->getCoins(), 0);
    p1->tax();
    CHECK_THROWS_AS_MESSAGE(p2->investCoins(0), std::invalid_argument ,"You have no 3 coins to invetigate");
    p2->tax();
    p1->tax();
    p2->tax();
    p1->gather();
    p2->investCoins(0);
    CHECK_EQ(p2->getCoins(), 7);
    CHECK_EQ(game->getCurrTurn(), 0);
}

TEST_CASE_FIXTURE(BeforeEach, "Spy button"){
    Spy* p1 = new Spy("Tal", Role::Spy, 0);
    game->setPlayers(*p1);
    Baron* p2 = new Baron("Ran", Role::Baron, 1);
    game->setPlayers(*p2);
    p1->preventArrest(p2->getPlayerNum());
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->tax();
    CHECK_THROWS_AS_MESSAGE(p2->arrest(p1->getPlayerNum()), std::invalid_argument ,"Spy prevent your arrest action");

}

TEST_CASE_FIXTURE(BeforeEach, "Merchant case"){
    Merchant* p1 = new Merchant("Tal", Role::Merchant, 0);
    game->setPlayers(*p1);
    Baron* p2 = new Baron("Ran", Role::Baron, 1);
    game->setPlayers(*p2);
    p1->increaseMerchantCoins();
    CHECK_EQ(p1->getCoins(), 0);
    p1->tax();
    p2->gather();
    p1->gather();
    p2->gather();
    p1->gather();
    p1->increaseMerchantCoins();
    CHECK_EQ(p1->getCoins(), 5);
}

TEST_CASE_FIXTURE(BeforeEach, "Game: nextTurn- with bribe action"){
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    p1->tax();
    p2->tax();
    p1->tax();
    p2->tax();
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->bribe();
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->tax();
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->tax();
    CHECK_EQ(game->getCurrTurn(), 1);
    p2->arrest(p1->getPlayerNum());
    p2->setPreventBribe(true);
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->bribe();
    CHECK_EQ(game->getCurrTurn(), 0);
    p1->tax();
    CHECK_EQ(game->getCurrTurn(), 1);
}

TEST_CASE_FIXTURE(BeforeEach, "Game: nextTurn - if player couped"){
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    General* p3 = new General("Shai", Role::General, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->cuop(p2->getPlayerNum());
    CHECK_EQ(game->getCurrTurn(), 2);
}

TEST_CASE_FIXTURE(BeforeEach, "Game: isCoup"){
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    General* p3 = new General("Shai", Role::General, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    CHECK_FALSE(game->isCouped(1));
    p1->cuop(p2->getPlayerNum());
    CHECK(game->isCouped(1));
}


TEST_CASE_FIXTURE(BeforeEach, "Game: isAnyPlayers"){
    CHECK_FALSE(game->isAnyPlayers());
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    CHECK(game->isAnyPlayers());
}

TEST_CASE_FIXTURE(BeforeEach, "Game: setPlayers"){
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    CHECK(game->getPlayersList()[0] == p1);
}

TEST_CASE_FIXTURE(BeforeEach, "Game: getCurrNumOfPlayers"){
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    General* p3 = new General("Shai", Role::General, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    CHECK_EQ(game->getCurrNumOfPlayers() , 3);
    p3->tax();
    p1->cuop(p2->getPlayerNum());
    CHECK_EQ(game->getCurrNumOfPlayers() , 2);
}

TEST_CASE_FIXTURE(BeforeEach, "Game: gameWiner"){
    if(game != nullptr){
        game->destroyInstance();
    }
    Game* game = game->getInstance(3);
    Governor* p1 = new Governor("Tal", Role::Governor, 0);
    game->setPlayers(*p1);
    Judge* p2 = new Judge("Ran", Role::Judge, 1);
    game->setPlayers(*p2);
    General* p3 = new General("Shai", Role::General, 2);
    game->setPlayers(*p3);
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    p3->tax();
    p1->tax();
    p2->gather();
    CHECK(game->gameWiner() < 0);
    p3->tax();
    p1->cuop(p2->getPlayerNum());
    CHECK(game->gameWiner() < 0);
    p3->tax();
    p1->tax();
    p3->cuop(p1->getPlayerNum());
    CHECK(game->gameWiner() >= 0);

}

TEST_CASE_FIXTURE(BeforeEach, "Baron: investCoins"){
    Baron p1 = Baron("Tal", Role::Baron, 0);
    game->setPlayers(p1);
    Judge p2 = Judge("Ran", Role::Judge, 1);
    game->setPlayers(p2);
    CHECK_THROWS_AS_MESSAGE(p1.investCoins(0), std::invalid_argument ,"You have no 3 coins to invetigate");
    p1.tax();
    p2.tax();
    p1.tax();
    p2.gather();
    p1.investCoins(0);
    CHECK_EQ(p1.getCoins(), 7);
}