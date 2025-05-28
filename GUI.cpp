//taliyariel1@gmail.com
#include "Game.hpp"
#include "Player.hpp"
#include "GUI.hpp"
#include <iostream>
#include <string>
#include <cstdlib> 
#include <ctime>   
#include <random>
#include "Governor.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
#include "Spy.hpp"

//Constructor for the GUI class.
//Initializes the main game window to half the desktop size,
//loads the required font, and initializes the start screen with player number selection buttons.
GUI::GUI() {
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    unsigned int width = desktop.width / 2;
    unsigned int height = desktop.height / 2;

    window.create(sf::VideoMode(width, height), "GUI Window", sf::Style::Close);

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font\n";
    }

    initStartScreen(width, height);
}

//Initializes the start screen buttons for selecting the number of players.
//Creates and positions 5 buttons labeled from 2 to 6 players.
void GUI::initStartScreen(unsigned int width, unsigned int height) {
    float buttonWidth = 60.0f;
    float buttonHeight = 60.0f;
    float spacing = 20.0f;
    float totalWidth = 6 * buttonWidth + 5 * spacing;
    float startX = (width - totalWidth) / 2.0f;
    float y = height / 2.0f;

    for (int i = 0; i < 5; ++i) {
        sf::RectangleShape rect(sf::Vector2f(buttonWidth, buttonHeight));
        rect.setFillColor(sf::Color(100, 100, 255));
        rect.setPosition(startX + i * (buttonWidth + spacing), y);
        startButtons.push_back(rect);

        sf::Text label(std::to_string(i + 2), font, 24);
        label.setFillColor(sf::Color::White);
        label.setPosition(rect.getPosition().x + 20, rect.getPosition().y + 10);
        startLabels.push_back(label);
    }
}

//Initializes the "Role" button used for assigning roles to players.
//Positions the button at the top center of the window and sets its label.
//The button is initially hidden and the click counter is reset.
void GUI::initRoleButton(unsigned int width) {
    float buttonWidth = 100.0f;
    float buttonHeight = 40.0f;
    float x = width / 2.0f - buttonWidth / 2.0f;
    float y = 50.0f;

    roleButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    roleButton.setFillColor(sf::Color(200, 100, 100));
    roleButton.setPosition(x, y);

    roleLabel.setFont(font);
    roleLabel.setString("Role");
    roleLabel.setCharacterSize(20);
    roleLabel.setFillColor(sf::Color::White);
    roleLabel.setPosition(x + 25, y + 5);

    roleClicksRemaining = 0;
    showRoleButton = false;
}

//Initializes the player name input buttons and labels.
//Clears any existing player-related data and creates a button and label for each player.
//Sets default names ("Player 1", "Player 2", etc.) and positions the buttons.
//Also initializes the Role and Enter buttons.
void GUI::initPlayerButtons(unsigned int width, unsigned int height) {
    playerButtons.clear();
    playerLabels.clear();
    playerNames.clear();
    playerLocked.clear();
    roleTexts.clear();
    assignedRoles.clear();

    float buttonWidth = width / 8.0f;
    float buttonHeight = 40.0f;
    float spacing = (width - buttonWidth * numPlayers) / (numPlayers + 1.0f);

    for (int i = 0; i < numPlayers; ++i) {
        sf::RectangleShape rect(sf::Vector2f(buttonWidth, buttonHeight));
        rect.setFillColor(sf::Color::Green);
        float y = height / 2.0f + 100; // מעט מתחת לאמצע
        rect.setPosition(spacing + i * (buttonWidth + spacing), y);
        playerButtons.push_back(rect);
        playerLocked.push_back(false);

        playerNames.push_back("Player " + std::to_string(i + 1));

        sf::Text text(playerNames[i], font, 16);
        text.setFillColor(sf::Color::Black);
        text.setPosition(rect.getPosition().x + 5, rect.getPosition().y + 5);
        playerLabels.push_back(text);
    }

    initRoleButton(width);
    initEnterButton(width);
}

//Initializes the center action buttons available during gameplay.
//Creates and positions 6 buttons labeled with action names:
//"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Cuop".
void GUI::initCenterButtons(unsigned int width, unsigned int height) {
    centerButtons.clear();
    actionLabels.clear();

    float buttonWidth = width / 8.0f;
    float buttonHeight = 40.0f;
    float spacing = (width - buttonWidth * 6) / 7.0f;
    float y = height / 2.0f - buttonHeight / 2.0f;
    std::string arr[6] = {"Gather", "Tax", "Bribe", "Arrest", "Sanction", "Cuop"};
    for (int i = 0; i < 6; ++i) {
        sf::RectangleShape rect(sf::Vector2f(buttonWidth, buttonHeight));
        rect.setFillColor(sf::Color::Blue);
        rect.setPosition(spacing + i * (buttonWidth + spacing), y);
        centerButtons.push_back(rect);

        sf::Text text(arr[i], font, 23);
        text.setFillColor(sf::Color::White);
        text.setPosition(rect.getPosition().x + 10, rect.getPosition().y + 5);
        actionLabels.push_back(text);
    }
}

//Initializes the "Enter" button for confirming actions.
//Places the button at the top center of the window and sets its appearance and label.
void GUI::initEnterButton(unsigned int width) {
    float buttonWidth = 100.0f;
    float buttonHeight = 40.0f;
    float x = width / 2.0f - buttonWidth / 2.0f;
    float y = 10.0f;

    enterButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
    enterButton.setFillColor(sf::Color(50, 150, 50));
    enterButton.setPosition(x, y);

    enterLabel.setFont(font);
    enterLabel.setString("Enter");
    enterLabel.setCharacterSize(20);
    enterLabel.setFillColor(sf::Color::White);
    enterLabel.setPosition(x + 20, y + 5);
}

//Checks if all player names are locked.
//Returns true if every player's name is marked as locked (confirmed), false otherwise.
bool GUI::allNamesLocked() {
    for (bool locked : playerLocked) {
        if (!locked) return false;
    }
    return true;
}

//Handle all Mouse press on the game window
void GUI::handleEvents() {
    //When game started, if remain one player show massage of the winer 
    if(showEnterButton){
        int winer = game->gameWiner();
        if(winer >= 0){
            showWinnerWindow(winer);
        }
    }
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed){
            game->destroyInstance();
            window.close();
        }
        //Handle mousw pressed
        else if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            //Check if governor press Yes/No on preventing tax action
            for (GovernorResponse& gr : governorResponses) {
                if (!gr.visible) continue;
                if (gr.yesButton.getGlobalBounds().contains(mousePos)) {
                    if(!preventedOnce){
                        Governor* gov = dynamic_cast<Governor*>(game->getPlayersList()[gr.playerIndex]);
                        gov->setPreventTax(true);
                        preventedOnce = true;
                        gr.visible = false;
                    }
                }
                else if (gr.noButton.getGlobalBounds().contains(mousePos)) {
                    gr.visible = false;
                }
            }
            preventedOnce = false;
            // After every response check if any more Governor need to respond
            bool allAnswered = true;
            for (const GovernorResponse& gr : governorResponses) {
                if (gr.visible) {
                    allAnswered = false;
                    break;
                }
            }
            //Check if general press Yes/No on preventing coup action
            for (GeneralResponse& glr : generalResponses) {
                if (!glr.visible) continue;
                if (glr.yesButton.getGlobalBounds().contains(mousePos)) {
                    if(!preventedOnceCoup){
                        General* g = static_cast<General*>(game->getPlayer(glr.playerIndex));
                        g->setPreventCoup(true);
                        preventedOnceCoup = true;
                        glr.visible = false;
                    }
                }
                else if (glr.noButton.getGlobalBounds().contains(mousePos)) {
                    glr.visible = false;
                }
            }
            preventedOnceCoup = false;
            bool allAnsweredGeneral = true;
            // After every response check if any more General need to respond
            for (const GeneralResponse& glr : generalResponses) {
                if (glr.visible) {
                    allAnsweredGeneral = false;
                    break;
                }
            }
            //Check if Judge press Yes/No on preventing bribe action
            for (JudgeResponse& jr : judgeResponses) {
                if (!jr.visible) continue;
                if (jr.yesButton.getGlobalBounds().contains(mousePos)) {
                    if(!preventedOnceBribe){
                        Judge* j = dynamic_cast<Judge*>(game->getPlayersList()[jr.playerIndex]);
                        j->setPreventBribe(true);
                        preventedOnceBribe = true;
                        jr.visible = false;
                    }
                }
                else if (jr.noButton.getGlobalBounds().contains(mousePos)) {
                    jr.visible = false;
                }
            }
            preventedOnceBribe = false;
            bool allAnsweredJudge = true;
            // After every response check if any more Judge need to respond
            for (const JudgeResponse& jr : judgeResponses) {
                if (jr.visible) {
                    allAnsweredJudge = false;
                    break;
                }
            }
            //If there no General players, do not wait for general respond on coup action
            if(showEnterButton){
                if(game->getNumOfGeneral() == 0 && coupedChosen){
                    allAnsweredGeneral = true;
                    lowNumOfGeneral = true;
                    coupedChosen = false;
                }
            }
            //If there no Judge players, do not wait for Judge respond on mribe action
            if(showEnterButton){
                if(game->getNumOfJudge() == 0){
                    allAnsweredJudge = true;
                    lowNumOfJudge = true;
                }
            }
            //If all responses absorbed, action can be played and all responses clean up
            if (allAnswered && !governorResponses.empty()) {
                try{game->getCurrPlayer()->tax();}
                catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                governorResponses.clear(); 
            }
            if (allAnsweredGeneral && !generalResponses.empty() && coupedChosen) {
                try{game->getCurrPlayer()->cuop(coupChosenPlayer);}
                catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                generalResponses.clear(); 
                coupedChosen = false;
            }
            if (allAnsweredJudge && !judgeResponses.empty()) {
                try{game->getCurrPlayer()->bribe();}
                catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                judgeResponses.clear(); 
            }
            if (state == StartScreen) {
                for (size_t i = 0; i < startButtons.size(); ++i) {
                    if (startButtons[i].getGlobalBounds().contains(mousePos)) {
                        numPlayers = static_cast<int>(i) + 2;
                        initPlayerButtons(window.getSize().x, window.getSize().y);
                        initCenterButtons(window.getSize().x, window.getSize().y);
                        state = GameScreen;
                        game = Game::getInstance(numPlayers);
                        break;
                    }
                }
            } else if (state == GameScreen) {
                activePlayerIndex = -1;
                for (size_t i = 0; i < playerButtons.size(); ++i) {
                    if (playerButtons[i].getGlobalBounds().contains(mousePos) && !playerLocked[i]) {
                        activePlayerIndex = static_cast<int>(i);
                        if (playerNames[i].rfind("Player", 0) == 0) {
                            playerNames[i].clear();
                            playerLabels[i].setString("");
                        }
                        break;
                    }
                }
                //Check if action button got pressed
                for (size_t i = 0; i < centerButtons.size(); ++i) {
                    if (centerButtons[i].getGlobalBounds().contains(mousePos)) {
                        //If there are players in the game, and the player that take turn press, and no action cosen yet- make action
                        if (game->isAnyPlayers() && game->getCurrTurn() == game->getCurrPlayer()->getPlayerNum() && selectedActionIndex == -1) {
                            //If Merchant take a turn and start it with 3 or more coins- get one more free coin
                            if(game->getCurrPlayer()->getRole() == Role::Merchant){
                                Merchant* m = dynamic_cast<Merchant*>(game->getCurrPlayer());
                                m->increaseMerchantCoins();
                            }
                            selectedActionIndex = static_cast<int>(i);
                            //If player starts his turn with 10 or more coins, he must do coup action
                            if(game->getCurrPlayer()->getCoins() >= 10){
                                coupForced = true;
                                selectedActionIndex = 5;
                            }
                            actionPerformed = true;
                        }
                    }
                }
                //Handle enter button pressed
                if (showEnterButton && enterButton.getGlobalBounds().contains(mousePos)) {
                    //If enter button pressed after all responses that had to absorbed happend and an action chosen and  the player that take turn press, then make the action
                    if (selectedActionIndex != -1 && game->getCurrTurn() == game->getCurrPlayer()->getPlayerNum() && allAnswered && allAnsweredGeneral && allAnsweredJudge && !coupForced) {
                        int currPlayer = game->getCurrPlayer()->getPlayerNum();
                        switch (selectedActionIndex) {
                            case 0: {try{game->getCurrPlayer()->gather();} 
                                    catch (const std::invalid_argument& e) {showErrorPopup(e.what());} break;}
                            //Cheack if the current player is the only Governor or if there are no Governors
                            case 1:{ if((game->getNumOfGovernor() == 1 && game->getCurrPlayer()->getRole() == Role::Governor) || game->getNumOfGovernor() == 0){
                                    try{game->getCurrPlayer()->tax();} 
                                    catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                            }
                            else{promptGovernorsForTaxBlock(currPlayer);}
                                break;
                                }
                            //Cheack if the current player is the only Judge or if there are no Judge
                            case 2: {if((game->getCurrPlayer()->getRole() == Role::Judge && game->getNumOfJudge() == 1) || game->getNumOfJudge() == 0){
                                        try{game->getCurrPlayer()->bribe(); } 
                                        catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                                    }
                                    else{
                                        promptJudgeForBribeBlock(currPlayer);
                                    }
                                    break;}
                            case 3:{ try{game->getCurrPlayer()->arrest(arrestChosenPlayer);}
                                    catch (const std::invalid_argument& e) {showErrorPopup(e.what());} break;}
                            case 4: {try{game->getCurrPlayer()->sanction(sunctionedChosenPlayer); }
                                    catch (const std::invalid_argument& e) {showErrorPopup(e.what());} break;}
                            //Cheack if the current player is the only General or if there are no General
                            case 5: { if((game->getCurrPlayer()->getRole() == Role::General && game->getNumOfGeneral() == 1) || game->getNumOfGeneral() == 0){
                                        try{game->getCurrPlayer()->cuop(coupChosenPlayer);}
                                        catch (const std::invalid_argument& e) {showErrorPopup(e.what());}
                                        coupedChosen = false;
                                      }
                                      else{
                                        promptGeneralForCuopBlock(currPlayer);
                                      }
                                    break;}
                        }
                        if(lowNumOfGovernor){
                            lowNumOfGovernor = false;
                        }
                        if(lowNumOfGeneral){
                            lowNumOfGeneral = false;
                        }
                        selectedActionIndex = -1; // אפס את הבחירה
                            actionPerformed = true;
                    }
                }
                if(arrestedChosen){
                    arrestedChosen = false;
                }
                if(sunctionedChosen){
                    sunctionedChosen = false;
                }
                //Handle role button press, give every player a role randomly and build the player
                if (showRoleButton && roleButton.getGlobalBounds().contains(mousePos)) {
                    if (roleClicksRemaining > 0) {
                        //srand(time(nullptr));
                        std::random_device rd;  
                        std::mt19937 gen(rd());
                        std::uniform_int_distribution<> dist(0, 5);
                        int randomIndex = dist(gen);
                        //int randomIndex = std::rand() % 6;
                        Role selectedRole = static_cast<Role>(randomIndex);
                        Player p = factory(selectedRole);

                        assignedRoles.push_back(selectedRole);

                        sf::Text roleText(to_string(selectedRole), font, 16);
                        roleText.setFillColor(sf::Color::Red);
                        float x = playerButtons[numPlayers - roleClicksRemaining].getPosition().x;
                        float y = playerButtons[numPlayers - roleClicksRemaining].getPosition().y - 20;
                        roleText.setPosition(x, y);
                        roleTexts.push_back(roleText);
                        roleClicksRemaining--;
                        if (roleClicksRemaining <= 0) {
                            showRoleButton = false;
                            showEnterButton = true;
                        }

                        //If any player is Spy, button of Spy action build
                        if (selectedRole == Role::Spy) {
                            sf::RectangleShape btn(sf::Vector2f(100, 30));
                            int index = game->getnumOfPlayers() - roleClicksRemaining - 1;
                           
                            float px = playerButtons[index].getPosition().x + playerButtons[index].getSize().x / 2.0f - 50;
                            float py = playerButtons[index].getPosition().y + playerButtons[index].getSize().y + 10;
                        
                            btn.setPosition(px, py);
                            btn.setFillColor(sf::Color(150, 0, 0));
                            spyActionButtons.push_back(btn);
                        
                            sf::Text lbl("Spy action", font, 14);
                            lbl.setFillColor(sf::Color::Magenta);
                            lbl.setPosition(px + 10, py + 5);
                            spyActionLabels.push_back(lbl);
                            showSpyActionButton = true;
                        }

                        //If any player is Baron, button of Baron action build
                        if (selectedRole == Role::Baron) {
                            sf::RectangleShape btn(sf::Vector2f(100, 30));
                            int index = game->getnumOfPlayers() - roleClicksRemaining - 1;
                           
                            float px = playerButtons[index].getPosition().x + playerButtons[index].getSize().x / 2.0f - 50;
                            float py = playerButtons[index].getPosition().y + playerButtons[index].getSize().y + 10;
                        
                            btn.setPosition(px, py);
                            btn.setFillColor(sf::Color(150, 0, 0));
                            baronActionButtons.push_back(btn);
                        
                            sf::Text lbl("Invest 3 coins", font, 14);
                            lbl.setFillColor(sf::Color::Magenta);
                            lbl.setPosition(px + 10, py + 5);
                            baronActionLabels.push_back(lbl);
                            showBaronActionButton = true;
                        }
                        
                    }
                }
                //Handle arrest action chosen, the player shuld choose other player for arresting
                if (selectedActionIndex == 3) { 
                    arrestedChosen = false;
                    actionPerformed = false;
                    for (int i = 0; i < game->getnumOfPlayers(); ++i) {
                        if (playerButtons[i].getGlobalBounds().contains(mousePos)) {
                            arrestChosenPlayer = i;
                            actionPerformed = true;       
                            showEnterButton = true; 
                            arrestedChosen = true;
                            break;
                        }
                    }
                }
                //Handle sanction action chosen, the player shuld choose other player for sanction
                if (selectedActionIndex == 4) { 
                    sunctionedChosen = false;
                    actionPerformed = false;
                    for (int i = 0; i < game->getnumOfPlayers(); ++i) {
                        if (playerButtons[i].getGlobalBounds().contains(mousePos)) {
                            sunctionedChosenPlayer = i;
                            actionPerformed = true;      
                            showEnterButton = true;   
                            sunctionedChosen = true;
                            break;
                        }
                    }
                }
                //Handle coup action chosen, the player shuld choose other player for coup
                if (selectedActionIndex == 5) {
                    coupedChosen = false;
                    actionPerformed = false;
                    for (int i = 0; i < game->getnumOfPlayers(); ++i) {
                        if (playerButtons[i].getGlobalBounds().contains(mousePos)) {
                            coupChosenPlayer = i;
                            actionPerformed = true;       
                            showEnterButton = true;      
                            coupedChosen = true;
                            coupForced = false;
                            break;
                        }
                    }
                }
                //Spy pressed spy action, see chosen player coins && arrest him
                for (size_t i = 0; i < spyActionButtons.size(); ++i) {
                    if (spyActionButtons[i].getGlobalBounds().contains(mousePos)) {
                        awaitingSpyTarget = true;
                        spyPlayerIndex = static_cast<int>(i);
                    }
                }
                int playerSpyNum = -1;
                if(showEnterButton && awaitingSpyTarget){
                    playerSpyNum = Spy::playerNumOfPressSpy(spyPlayerIndex);
                }
                
                    if(awaitingSpyTarget){
                        for (int i = 0; i < game->getnumOfPlayers(); ++i) {
                            //Ceack if spy prresed himself for arrest or if this Spy is allredy couped, both cant be arrested
                            if (i == playerSpyNum) continue;
                            if(game->isCouped(playerSpyNum)){
                                awaitingSpyTarget = false;
                                break;
                            }
                            //Choos player for see his coins && arrest action
                            if (playerButtons[i].getGlobalBounds().contains(mousePos)) {
                                //Check if spy pressed couped player
                                if( !game->isCouped(i)){
                                int playerCoins = game->getPlayer(i)->getCoins();
                                std::string std = "Coins of player" + std::to_string(i + 1) + ": " + std::to_string(playerCoins);
                                float px = playerButtons[i].getPosition().x;
                                float py = playerButtons[i].getPosition().y + 100;
                                showTemporaryMessage(std, font, px , py);
                                Spy* s = static_cast<Spy*>(game->getPlayersList()[playerSpyNum]);
                                s->preventArrest(i);
                                awaitingSpyTarget = false;
                                spyPlayerIndex = -1;
                                break;
                                }
                                else{
                                    awaitingSpyTarget = false;
                                }
                            }
                        }
                    }
                    //Hendle Baron button press
                    for (int i = 0; i < baronActionButtons.size(); ++i) {
                        if (baronActionButtons[i].getGlobalBounds().contains(mousePos)) {
                            //if the baron that antered is the baron that take turn now.
                            if(game->getCurrPlayer()->getRole() == Role::Baron){
                                Baron* b = static_cast<Baron*>(game->getCurrPlayer());
                                try{
                                    b->investCoins(i);
                                }
                                catch (const std::invalid_argument& e) {
                                    showErrorPopup(e.what());
                                }
                                
                            }
                        }
                    }
            }
        }

        else if (event.type == sf::Event::TextEntered && activePlayerIndex != -1 && state == GameScreen) {
            if (event.text.unicode == 13) {
                if (actionPerformed && governorResponses.empty()) {
                    actionPerformed = false; // איפוס
                }
                playerLocked[activePlayerIndex] = true;
                activePlayerIndex = -1;

                if (allNamesLocked()) {
                    showRoleButton = true;
                    roleClicksRemaining = numPlayers;
                }
            }
            else if (event.text.unicode == 8 && !playerNames[activePlayerIndex].empty()) {
                playerNames[activePlayerIndex].pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode <= 126 && playerNames[activePlayerIndex].size() < 12) {
                char typedChar = static_cast<char>(event.text.unicode);
                playerNames[activePlayerIndex] += typedChar;
            }

            if (activePlayerIndex != -1)
                playerLabels[activePlayerIndex].setString(playerNames[activePlayerIndex]);
        }
    }
}

//Factory method that creates and returns a Player-derived object based on the given role.
//Assigns the created player to the game and updates role-specific counters.
Player& GUI::factory(Role role){
    int index = game->getnumOfPlayers() - roleClicksRemaining;
    if(role == Role::Governor){
        Governor* gov = new Governor(playerNames[index], Role::Governor,  index);
        game->setPlayers(*gov);
        game->increasNumOfGovernor();
        //numOfGovernor = numOfGovernor + 1;
        return *gov;
    } else if(role == Role::Spy){
        Spy* spy = new Spy(playerNames[index], Role::Spy,  index);
        game->setPlayers(*spy);
        return *spy;
    }else if(role == Role::Baron){
        Baron* bar = new Baron(playerNames[index], Role::Baron, index);
        game->setPlayers(*bar);
        return *bar;
    }else if(role == Role::General){
        General* gen = new General(playerNames[index], Role::General,  index);
        game->setPlayers(*gen);
        game->increasNumOfGeneral();
        //numOfGeneral = numOfGeneral + 1;
        return *gen;
    }else if(role == Role::Judge){
        Judge* jud = new Judge(playerNames[index], Role::Judge,  index);
        game->setPlayers(*jud);
        game->increasNumOfJudge();
        //numOfJudge = numOfJudge + 1;
        return *jud;
    }else if(role == Role::Merchant){
        Merchant* mer = new Merchant(playerNames[index], Role::Merchant,  index);
        game->setPlayers(*mer);
        return *mer;
    }
    return *(new Player("", role, 0));
}

//Prompts all Governors (except the taxing player) with a question whether to block a tax action.
//Displays Yes/No buttons for each eligible Governor.
void GUI::promptGovernorsForTaxBlock(int taxingPlayerIndex) {
    governorResponses.clear();
    for (int i = 0; i < game->getnumOfPlayers(); ++i) {
        //If player i is not the player that make tax action and he is Governor and hi is not couped
        if (i != taxingPlayerIndex && assignedRoles[i] == Role::Governor && !game->isCouped(i)) {
            GovernorResponse gr;
            gr.playerIndex = i;

            // question
            gr.question.setFont(font);
            gr.question.setString("Prevent tax?");
            gr.question.setCharacterSize(14);
            gr.question.setFillColor(sf::Color::Black);

            // position
            float px = playerButtons[i].getPosition().x;
            float py = playerButtons[i].getPosition().y + 50;

            gr.question.setPosition(px, py);

            //Yes button
            gr.yesButton.setSize(sf::Vector2f(40, 25));
            gr.yesButton.setFillColor(sf::Color::Green);
            gr.yesButton.setPosition(px, py + 20);

            gr.yesLabel.setFont(font);
            gr.yesLabel.setString("Yes");
            gr.yesLabel.setCharacterSize(12);
            gr.yesLabel.setFillColor(sf::Color::White);
            gr.yesLabel.setPosition(px + 8, py + 23);

            //No button
            gr.noButton.setSize(sf::Vector2f(40, 25));
            gr.noButton.setFillColor(sf::Color::Red);
            gr.noButton.setPosition(px + 50, py + 20);

            gr.noLabel.setFont(font);
            gr.noLabel.setString("No");
            gr.noLabel.setCharacterSize(12);
            gr.noLabel.setFillColor(sf::Color::White);
            gr.noLabel.setPosition(px + 58, py + 23);

            gr.visible = true;
            governorResponses.push_back(gr);
        }
    }
}

//Prompts all Generals with 5 or more coins with a question whether to block a coup action.
//Displays Yes/No buttons for each eligible General.
void GUI::promptGeneralForCuopBlock(int coupingPlayerIndex){
    generalResponses.clear();
    for (int i = 0; i < game->getnumOfPlayers(); ++i) {
        //If player i is not the player that make coup action and he is General and he is not couped
        if (i != coupingPlayerIndex && assignedRoles[i] == Role::General && !game->isCouped(i)) {
            if(game->getPlayer(i)->getCoins() >= 5){
                GeneralResponse glr;
                glr.playerIndex = i;

                // question
                glr.question.setFont(font);
                glr.question.setString("Prevent coup?");
                glr.question.setCharacterSize(14);
                glr.question.setFillColor(sf::Color::Black);

                // position
                float px = playerButtons[i].getPosition().x;
                float py = playerButtons[i].getPosition().y + 50;

                glr.question.setPosition(px, py);

                //Yes button
                glr.yesButton.setSize(sf::Vector2f(40, 25));
                glr.yesButton.setFillColor(sf::Color::Green);
                glr.yesButton.setPosition(px, py + 20);

                glr.yesLabel.setFont(font);
                glr.yesLabel.setString("Yes");
                glr.yesLabel.setCharacterSize(12);
                glr.yesLabel.setFillColor(sf::Color::White);
                glr.yesLabel.setPosition(px + 8, py + 23);

                //No button
                glr.noButton.setSize(sf::Vector2f(40, 25));
                glr.noButton.setFillColor(sf::Color::Red);
                glr.noButton.setPosition(px + 50, py + 20);

                glr.noLabel.setFont(font);
                glr.noLabel.setString("No");
                glr.noLabel.setCharacterSize(12);
                glr.noLabel.setFillColor(sf::Color::White);
                glr.noLabel.setPosition(px + 58, py + 23);

                glr.visible = true;
                generalResponses.push_back(glr);
            }
        }
    }
    if(generalResponses.size() == 0){
        try{
            game->getPlayersList()[coupingPlayerIndex]->cuop(coupChosenPlayer);
        }
        catch (const std::invalid_argument& e) {
            showErrorPopup(e.what());
        }
    }
}

//Prompts all Judges with a question whether to block a bribe action.
//Displays Yes/No buttons for each eligible Judge.
void GUI::promptJudgeForBribeBlock(int bribingPlayerIndex){
        judgeResponses.clear();
        for (int i = 0; i < game->getnumOfPlayers(); ++i) {
            //If player i is not the player that make tax bribe and he is Judge and he is not couped
            if (i != bribingPlayerIndex && assignedRoles[i] == Role::Judge && !game->isCouped(i)) {
                JudgeResponse jr;
                jr.playerIndex = i;

                // question
                jr.question.setFont(font);
                jr.question.setString("Prevent bribe?");
                jr.question.setCharacterSize(14);
                jr.question.setFillColor(sf::Color::Black);

                // position
                float px = playerButtons[i].getPosition().x;
                float py = playerButtons[i].getPosition().y + 50;

                jr.question.setPosition(px, py);

                //Yes button
                jr.yesButton.setSize(sf::Vector2f(40, 25));
                jr.yesButton.setFillColor(sf::Color::Green);
                jr.yesButton.setPosition(px, py + 20);

                jr.yesLabel.setFont(font);
                jr.yesLabel.setString("Yes");
                jr.yesLabel.setCharacterSize(12);
                jr.yesLabel.setFillColor(sf::Color::White);
                jr.yesLabel.setPosition(px + 8, py + 23);

                //No button
                jr.noButton.setSize(sf::Vector2f(40, 25));
                jr.noButton.setFillColor(sf::Color::Red);
                jr.noButton.setPosition(px + 50, py + 20);

                jr.noLabel.setFont(font);
                jr.noLabel.setString("No");
                jr.noLabel.setCharacterSize(12);
                jr.noLabel.setFillColor(sf::Color::White);
                jr.noLabel.setPosition(px + 58, py + 23);

                jr.visible = true;
                judgeResponses.push_back(jr);
            }
        }
    }

//Displays a temporary text message on screen at the specified location.
//The message remains visible for 5 seconds.
void GUI::showTemporaryMessage(const std::string& message, const sf::Font& font, float x, float y) {
    temporaryText.setFont(font);
    temporaryText.setString(message);
    temporaryText.setCharacterSize(20);
    temporaryText.setFillColor(sf::Color::Black);
    temporaryText.setPosition(x, y);
    temporaryTextClock.restart();
    showTemporaryText = true;
}

//Function that open new window and show writ the number of the game winer player
void GUI::showWinnerWindow(int playerNumber) {
    sf::RenderWindow window(sf::VideoMode(400, 250), "Game Over");
    sf::Font font;
    
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font\n";
        return;
    }

    //Winer lable
    sf::Text text;
    text.setFont(font);
    text.setString("Player " + std::to_string(playerNumber + 1) + " wins the game!");
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Blue);
    text.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    text.setPosition(window.getSize().x / 2.0f, 70);

    // Exit buttom
    sf::RectangleShape exitButton(sf::Vector2f(100, 40));
    exitButton.setFillColor(sf::Color(200, 0, 0));
    exitButton.setPosition(window.getSize().x / 2.0f - 50, 150);

    // Exit button lable
    sf::Text exitText;
    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::White);
    sf::FloatRect exitTextRect = exitText.getLocalBounds();
    exitText.setOrigin(exitTextRect.width / 2.0f, exitTextRect.height / 2.0f);
    exitText.setPosition(exitButton.getPosition().x + exitButton.getSize().x / 2,
                         exitButton.getPosition().y + exitButton.getSize().y / 4);

    //display loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (exitButton.getGlobalBounds().contains(mousePos)) {
                        game->destroyInstance();
                        window.close();
                        exit(0);
                    }
                }
            }
        }

        window.clear(sf::Color(173, 216, 230));
        window.draw(text);
        window.draw(exitButton);
        window.draw(exitText);
        window.display();
    }
}

//Function for showing errors
void GUI::showErrorPopup(const std::string& message) {
    sf::RenderWindow errorWindow(sf::VideoMode(400, 150), "Error");
    sf::Font font;

    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font for error popup.\n";
        return;
    }

    sf::Text errorText;
    errorText.setFont(font);
    errorText.setString(message);
    errorText.setCharacterSize(14);
    errorText.setFillColor(sf::Color::Red);
    errorText.setStyle(sf::Text::Bold);
    sf::FloatRect textRect = errorText.getLocalBounds();
    errorText.setOrigin(textRect.width / 2, textRect.height / 2);
    errorText.setPosition(errorWindow.getSize().x / 2, errorWindow.getSize().y / 2 - 20);

    // Enter buttom
    sf::RectangleShape enterButton(sf::Vector2f(100, 40));
    enterButton.setFillColor(sf::Color(200, 0, 0));
    enterButton.setPosition(errorWindow.getSize().x / 2 - 50, errorWindow.getSize().y / 2 + 30);

    // Enter button lable
    sf::Text enterText;
    enterText.setFont(font);
    enterText.setString("Enter");
    enterText.setCharacterSize(20);
    enterText.setFillColor(sf::Color::White);
    sf::FloatRect enterTextRect = enterText.getLocalBounds();
    enterText.setOrigin(enterTextRect.width / 2.0f, enterTextRect.height / 2.0f);
    enterText.setPosition(enterButton.getPosition().x + enterButton.getSize().x / 2, enterButton.getPosition().y + enterButton.getSize().y / 2);

    while (errorWindow.isOpen()) {
        sf::Event event;
        while (errorWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed){
                //selectedActionIndex = -1;
                errorWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                    if (enterButton.getGlobalBounds().contains(mousePos)) {
                        //selectedActionIndex = -1;
                        errorWindow.close();
                        
                    }
                }
            }
        }

        errorWindow.clear(sf::Color::White); // רקע כהה
        errorWindow.draw(errorText);
        errorWindow.draw(enterButton);
        errorWindow.draw(enterText);
        errorWindow.display();
    }
}

//Renders the entire game interface based on the current GUI state.
//This includes player buttons, role assignments, action buttons, response prompts, and temporary messages.
void GUI::render() {
    window.clear(sf::Color(173, 216, 230));

    if (state == StartScreen) {
        sf::Text title("Choose number of players", font, 30);
        title.setFillColor(sf::Color::Black);
        title.setPosition(window.getSize().x / 2.0f - 150, 100);
        window.draw(title);

        for (size_t i = 0; i < startButtons.size(); ++i) {
            window.draw(startButtons[i]);
            window.draw(startLabels[i]);
        }
    } else {
        for (size_t i = 0; i < playerButtons.size(); ++i) {
            if (static_cast<int>(i) == activePlayerIndex)
                playerButtons[i].setFillColor(sf::Color::Yellow);
            //Mark the bo=utton of the player that take a turn in red
            else if (game->getCurrTurn() == static_cast<int>(i) && (roleClicksRemaining == 0 && allNamesLocked()))
                playerButtons[i].setFillColor(sf::Color::Red);
            else
                playerButtons[i].setFillColor(sf::Color::Green);

            window.draw(playerButtons[i]);
            window.draw(playerLabels[i]);
        }

        for (size_t i = 0; i < roleTexts.size(); ++i) {
            window.draw(roleTexts[i]);
        }

        if (showRoleButton) {
            window.draw(roleButton);
            window.draw(roleLabel);
        }

        for (size_t i = 0; i < centerButtons.size(); ++i) {
            window.draw(centerButtons[i]);
            window.draw(actionLabels[i]);
        }
        for (const GovernorResponse& gr : governorResponses) {
            if (gr.visible) {
                window.draw(gr.question);
                window.draw(gr.yesButton);
                window.draw(gr.yesLabel);
                window.draw(gr.noButton);
                window.draw(gr.noLabel);
            }
        }
        for (const GeneralResponse& glr : generalResponses) {
            if (glr.visible) {
                window.draw(glr.question);
                window.draw(glr.yesButton);
                window.draw(glr.yesLabel);
                window.draw(glr.noButton);
                window.draw(glr.noLabel);
            }
        }
        for (const JudgeResponse& jr : judgeResponses) {
            if (jr.visible) {
                window.draw(jr.question);
                window.draw(jr.yesButton);
                window.draw(jr.yesLabel);
                window.draw(jr.noButton);
                window.draw(jr.noLabel);
            }
        }
        //Show player coins for 5 seconds by spy action
        if (showTemporaryText) {
            if (temporaryTextClock.getElapsedTime().asSeconds() < 5.0f) {
                window.draw(temporaryText);
            } else {
                showTemporaryText = false;
            }
        }
    }
    if (showEnterButton) {
        window.draw(enterButton);
        window.draw(enterLabel);
    }
    //Show spy action button
    if(showSpyActionButton){
        for (const auto& btn : spyActionButtons)
            window.draw(btn);

        for (const auto& lbl : spyActionLabels)
            window.draw(lbl);
    }
    //Show baron action button
    if(showBaronActionButton){
        for (const auto& btn : baronActionButtons)
            window.draw(btn);

        for (const auto& lbl : baronActionLabels)
            window.draw(lbl);
    }
    window.display();
}

//Runs the main loop of the GUI, continuously handling events and rendering the interface.
//Keeps the window open until the user closes it.
void GUI::run() {
    while (window.isOpen()) {
        handleEvents();
        render();
    }
}