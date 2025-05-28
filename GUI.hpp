//taliyariel1@gmail.com
#ifndef GUI_HPP
#define GUI_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "Game.hpp"

class GUI {
private:
    Game* game;
    enum GameState { StartScreen, GameScreen };
    GameState state = StartScreen;
    sf::RenderWindow window;
    sf::Font font;

public:
    GUI();
    void run();

    // Start screen
    std::vector<sf::RectangleShape> startButtons;
    std::vector<sf::Text> startLabels;

    // Game screen
    int numPlayers = 0;
    std::vector<sf::RectangleShape> playerButtons;
    std::vector<sf::Text> playerLabels;
    std::vector<std::string> playerNames;
    std::vector<bool> playerLocked;

    // Role display
    sf::RectangleShape roleButton;
    sf::Text roleLabel;
    bool showRoleButton = false;
    int roleClicksRemaining = 0;
    std::vector<Role> assignedRoles;
    std::vector<sf::Text> roleTexts;

    // Actions
    std::vector<sf::RectangleShape> centerButtons;
    std::vector<sf::Text> actionLabels;

    int activePlayerIndex = -1;

    // Initialization
    void initStartScreen(unsigned int width, unsigned int height);
    void initPlayerButtons(unsigned int width, unsigned int height);
    void initRoleButton(unsigned int width);
    void initCenterButtons(unsigned int width, unsigned int height);
    void initEnterButton(unsigned int width);

    // Logic
    void handleEvents();
    void render();
    bool allNamesLocked();
    Player& factory(Role role);
    void showWinnerWindow(int playerNumber);
    void showErrorPopup(const std::string& message);

    //Enter button
    sf::RectangleShape enterButton;
    sf::Text enterLabel;
    bool showEnterButton = false;

    int selectedActionIndex = -1;

    //Spy action button
    std::vector<sf::RectangleShape> spyActionButtons;
    std::vector<sf::Text> spyActionLabels;
    bool showSpyActionButton = false;

    //Spy action- see other palyers coins
    sf::Text temporaryText;
    sf::Clock temporaryTextClock;
    bool showTemporaryText = false;
    void showTemporaryMessage(const std::string& message, const sf::Font& font, float x, float y);
    bool awaitingSpyTarget = false;
    int spyPlayerIndex = -1;

    //Baron action button
    std::vector<sf::RectangleShape> baronActionButtons;
    std::vector<sf::Text> baronActionLabels;
    bool showBaronActionButton = false;

    //Governor tax preventation
    struct GovernorResponse {
        int playerIndex;
        sf::Text question;
        sf::RectangleShape yesButton;
        sf::Text yesLabel;
        sf::RectangleShape noButton;
        sf::Text noLabel;
        bool visible = false;
    };
    int numOfGovernor = 0;
    bool lowNumOfGovernor = false;
    bool actionPerformed = false;
    void promptGovernorsForTaxBlock(int taxingPlayerIndex);
    std::vector<GovernorResponse> governorResponses;

    //General coup preventation
    struct GeneralResponse {
        int playerIndex;
        sf::Text question;
        sf::RectangleShape yesButton;
        sf::Text yesLabel;
        sf::RectangleShape noButton;
        sf::Text noLabel;
        bool visible = false;
    };
    void promptGeneralForCuopBlock(int coupingPlayerIndex);
    std::vector<GeneralResponse> generalResponses;
    bool preventedOnceCoup = false;
    int numOfGeneral = 0;
    bool lowNumOfGeneral = false;

    //Judge bribe preventation
    struct JudgeResponse {
        int playerIndex;
        sf::Text question;
        sf::RectangleShape yesButton;
        sf::Text yesLabel;
        sf::RectangleShape noButton;
        sf::Text noLabel;
        bool visible = false;
    };
    void promptJudgeForBribeBlock(int bribingPlayerIndex);
    std::vector<JudgeResponse> judgeResponses;
    bool preventedOnceBribe = false;
    int numOfJudge = 0;
    bool lowNumOfJudge = false;
    bool preventedOnce = false;

    //Choos player for arresting
    bool arrestedChosen = false;
    int arrestChosenPlayer = 0;

    //Choos player for sanction
    bool sunctionedChosen = false;
    int sunctionedChosenPlayer = 0;

    //Choos player for coup
    bool coupedChosen = false;
    int coupChosenPlayer = -1;
    bool coupForced = false;

    //Set game for testing
    void setGame(Game* g) { game = g; }

};

#endif // GUI_HPP