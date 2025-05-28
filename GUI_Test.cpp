#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "GUI.hpp"
#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"

struct BeforeEach {
    public:
    int x;
    Game* game;
        BeforeEach():x(2) {
                Game::destroyInstance();
                game = Game::getInstance(x);
                REQUIRE(game != nullptr);
        }
    };

TEST_CASE("GUI initializes start buttons correctly") {
    GUI gui;
    REQUIRE(gui.startButtons.size() == 5);
    REQUIRE(gui.startLabels.size() == 5);

    for (int i = 0; i < 5; ++i) {
        REQUIRE(gui.startLabels[i].getString() == std::to_string(i + 2));
    }
}

TEST_CASE( "Role button is initialized correctly") {
    GUI gui;
    gui.initRoleButton(800);
    
    REQUIRE(gui.roleButton.getSize().x == Approx(100.0f));
    REQUIRE(gui.roleButton.getSize().y == Approx(40.0f));
    REQUIRE(gui.roleLabel.getString() == "Role");
}

TEST_CASE("Player buttons initialized according to number of players") {
    GUI gui;
    gui.numPlayers = 4;
    gui.initPlayerButtons(800, 600);

    REQUIRE(gui.playerButtons.size() == 4);
    REQUIRE(gui.playerLabels.size() == 4);
    REQUIRE(gui.playerNames.size() == 4);
    REQUIRE(gui.playerLocked.size() == 4);

    for (int i = 0; i < 4; ++i) {
        REQUIRE(gui.playerNames[i] == "Player " + std::to_string(i + 1));
        REQUIRE_FALSE(gui.playerLocked[i]);
    }
}

TEST_CASE("Center buttons initialized correctly") {
    GUI gui;
    gui.initCenterButtons(800, 600);
    
    REQUIRE(gui.centerButtons.size() == 6);
    REQUIRE(gui.actionLabels.size() == 6);
    
    std::string expected[] = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Cuop"};
    for (int i = 0; i < 6; ++i) {
        REQUIRE(gui.actionLabels[i].getString() == expected[i]);
    }
}

TEST_CASE("Enter button is initialized correctly") {
    GUI gui;
    gui.initEnterButton(800);

    REQUIRE(gui.enterButton.getSize().x == Approx(100.0f));
    REQUIRE(gui.enterButton.getSize().y == Approx(40.0f));
    REQUIRE(gui.enterLabel.getString() == "Enter");
}

TEST_CASE_METHOD(BeforeEach, "GUI::factory creates and registers the correct player types") {
    GUI gui;
    gui.numPlayers = 2;
    gui.playerNames = {"Alice", "Bob"};
    gui.roleClicksRemaining = 2;
    gui.assignedRoles.resize(2);

    Game* game = game->getInstance(2);
    gui.setGame(game); 

    Player& gov = gui.factory(Role::Governor);
    REQUIRE(gov.getRole() == Role::Governor);
    REQUIRE(gov.getName() == "Alice");
    //REQUIRE(game->getNumOfGovernor() == 1);

    gui.roleClicksRemaining = 1;
    Player& spy = gui.factory(Role::Spy);
    REQUIRE(spy.getRole() == Role::Spy);
    REQUIRE(spy.getName() == "Bob");

}

TEST_CASE_METHOD(BeforeEach, "promptGovernorsForTaxBlock shows responses for all governors except the taxing player") {
    GUI gui;
    game->destroyInstance();
    game = Game::getInstance(3);
    gui.setGame(game);
    gui.playerButtons.resize(3, sf::RectangleShape(sf::Vector2f(100, 40)));
    gui.assignedRoles = {Role::Governor, Role::Spy, Role::Governor};
    gui.initPlayerButtons(800, 600);
    Governor* g1 = new Governor("G1", Role::Governor, 0);
    Spy* g2 = new Spy("G1", Role::Spy, 0);
    Governor* g3 = new Governor("G2", Role::Governor, 2);
    game->setPlayers(*g1);
    game->setPlayers(*g2);
    game->setPlayers(*g3);
    // הגדרת מיקומים רנדומליים כדי למנוע גישה לערכים לא מאותחלים
    for (int i = 0; i < 3; ++i)
        gui.playerButtons[i].setPosition(100.f * i, 100.f);

    gui.promptGovernorsForTaxBlock(2); // השחקן השלישי מבצע Tax, הוא לא נשאל

    REQUIRE(gui.governorResponses.size() == 1);
    REQUIRE(gui.governorResponses[0].playerIndex == 0);
    REQUIRE(gui.governorResponses[0].question.getString() == "Prevent tax?");
    REQUIRE(gui.governorResponses[0].visible);
}

TEST_CASE_METHOD(BeforeEach, "promptGeneralForCuopBlock asks generals with 5+ coins") {
    GUI gui;
    game->destroyInstance();
    Game* game = Game::getInstance(3);
    gui.setGame(game);
    gui.assignedRoles = {Role::General, Role::Spy, Role::General};
    gui.playerButtons.resize(3, sf::RectangleShape(sf::Vector2f(100, 40)));

    for (int i = 0; i < 3; ++i)
        gui.playerButtons[i].setPosition(100.f * i, 100.f);

    General* g1 = new General("G1", Role::General, 0);
    General* g2 = new General("G2", Role::General, 2);
    game->setPlayers(*g1);
    game->setPlayers(*g2);
    g1->increaseCoins();
    g1->increaseCoins();
    g1->increaseCoins();
    g1->increaseCoins();
    g1->increaseCoins(); // eligible
    g2->increaseCoins();
    g2->increaseCoins();
    g2->increaseCoins(); // not eligible
    Player p = Player("S", Role::Spy, 1);
    game->setPlayers(p);
    
    gui.promptGeneralForCuopBlock(2);

    REQUIRE(gui.generalResponses.size() == 1);
    REQUIRE(gui.generalResponses[0].playerIndex == 0);
    REQUIRE(gui.generalResponses[0].question.getString() == "Prevent coup?");
    REQUIRE(gui.generalResponses[0].visible);
}

TEST_CASE("allNamesLocked returns correct value") {
    GUI gui;
    gui.playerLocked = {true, true, true};
    REQUIRE(gui.allNamesLocked());

    gui.playerLocked = {true, false, true};
    REQUIRE_FALSE(gui.allNamesLocked());
}

TEST_CASE("showTemporaryMessage sets temporaryText correctly") {
    GUI gui;
    sf::Font font;
    font.loadFromFile("arial.ttf"); // או כל פונט אחר שקיים

    gui.showTemporaryMessage("Hello", font, 100.f, 200.f);

    REQUIRE(gui.temporaryText.getString() == "Hello");
    REQUIRE(gui.temporaryText.getPosition().x == Approx(100.f));
    REQUIRE(gui.temporaryText.getPosition().y == Approx(200.f));
    REQUIRE(gui.showTemporaryText);
}

TEST_CASE("promptJudgeForBribeBlock shows judges correctly") {
    GUI gui;
    Game* game = Game::getInstance(2);
    gui.setGame(game);
    gui.assignedRoles = {Role::Judge, Role::Merchant, Role::Governor};
    gui.playerButtons.resize(2, sf::RectangleShape(sf::Vector2f(100, 40)));


    for (int i = 0; i < 2; ++i)
        gui.playerButtons[i].setPosition(100.f * i, 100.f);

    Judge* g1 = new Judge("G1", Role::Judge, 0);
    Merchant* g2 = new Merchant("G2", Role::Merchant, 1);
    game->setPlayers(*g1);
    game->setPlayers(*g2);

    gui.promptJudgeForBribeBlock(1);

    REQUIRE(gui.judgeResponses.size() == 1);
    REQUIRE(gui.judgeResponses[0].playerIndex == 0);
    REQUIRE(gui.judgeResponses[0].question.getString() == "Prevent bribe?");
    REQUIRE(gui.judgeResponses[0].visible);

    delete g1;
    delete g2;
}