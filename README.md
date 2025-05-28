##README FILE- COUP

##Project Overview
This is a C++ implementation of the strategic game Coup. The project supports 2 to 6 players and includes full game mechanics (GUI), turn management, and role-specific actions.
 The goal of each player in the game is to be the last one standing and thus be recognized as the winner.
 The logic of the game is encapsulated in the game manager class, and each player is assigned a role represented by a subclass of the player base class (such as governor, judge, spy, baron, General, and Merchant.).
The player class contains the 6 basic actions of the game, and each inheriting class, according to its role, performs additional and special actions according to the role it fulfills.

##Setup and Compilation
To compile and run the project, follow these steps:
1. Clone the repository.
2. Compile the code use a C++ compiler.
3. Options: 
make Main- to run the demo file.
make test- to run the unit test.
make valgrind- memory leak testing using valgrind.
make clean- deletes all irrelevant files after running.
4. When starting the game, the user will be required to write a name for each player and after writing each name, press the ENTER button on the keyboard.
 (Only after all ENTER presses have been pressed will the game be able to run.)
5. A red mark on a player's button will indicate that it is the marked player's turn.

##Class:
Game, Player, Baron, Spy, Merchant, Governor, General, Judge, GUI Test, GUI_Test

###Class: Game
Game is a singleton class.
Functions:
-Game - Private consrtactor - Initializes a game instance
- ~Game - Private distractor
- Game- Delete copy constractor 
- operator= - Delete operator
- getInstance - Returns the singleton instance of the game, creating it if needed.
- destroyInstance - Deletes and resets the game instance.
- setPlayers - Adds a player to the internal player list (up to numOfPlayers).
- nextTurn - Advances the game to the next turn, handles bribe logic and skips eliminated players.
- isCouped - Returns true if player i was couped.
- isAnyPlayers - Returns true if at least one player is in the game.
- gameWiner - Returns the number of the winning player if only one remains, otherwise -1.
- and other geters and seters to numOfGeneral, numOfGovernor, numOfJudge.

###Class: Player
Represents a player in the game and implements all game actions
Functions:
- Player- Constractor get name, role and player number
- gather - Gain 1 coin if not currently sanctioned.
- tax - Gain 2 coins if not sanctioned and not blocked by a Governor.
- bribe - Pay 4 coins to gain an extra turn (1 or 2 depending on whether a Judge blocks the action).
- arrest - Steal a coin from another player (unless blocked by Spy or if repeated arrest). Special effects for Merchant/General roles.
- sanction - Pay 3 coins to prevent another player from performing economic actions in their next turn.
- cuop - Pay 7 coins to eliminate (coup) another player unless blocked by a General.
- decreaseCoins - Reduces the player's coins by a specified amount.
- operator== - Equality operator for comparing two players by role, name, and ID.
- Copy constructor & assignment operator - Enable deep copying of player state.

###Class: Baron
Baron  can invest 3 coins and get 6 back, if get attack by sanction- get one coin free. Baron class inheritor player
Functions:
- Baron - Constractor get name, role and player number
- investCoins - Allows the Baron to invest 3 coins and receive 6 coins total (3 profit). This action is only available to the current Baron player who initiated the action.
- embargoAttack - Automatically triggered when the Baron is targeted with a sanction. The Baron gains 1 compensation coin.

###Class: Spy
Spy can expose other players coins and prevent other players arrest action at their next turn. Spy inheritor player.
Functions:
- Spy - Constractor get name, role and player number
- playerNumOfPressSpy - Identifies which Spy player (by index) initiated the action. Used internally to match GUI button presses to actual players.
- preventArrest - Prevents the selected player from being arrested during the current round.

###Class: Merchant
Merchant gets one coin free if started his turn with 3 coins, if attacked by arrest- paies 2 coins to cash box . Merchant inheritor player.
Functions:
- Merchant - Constractor get name, role and player number
- increaseMerchantCoins - Automatically grants the Merchant 1 additional coin at the start of their turn if they have at least 3 coins and haven’t already received this bonus in the current round.

###Class: Governor
Governor gets 3 coins by tax action, can prevent others tax action. Governor class inheritor player
Functions:
- Governor - Constractor get name, role and player number
- tax -  Overrides the base tax action to give the Governor 3 coins 
- isTaxPrevented - Checks if any Governor in the game has chosen to prevent a tax action. If so, prevents the action and resets their internal flag.

###Class: General
General can pay 5 coins for prevent coup action, if attacked by arrest- gets back his coin. General class inheritor player
Functions:
- General - Constractor get name, role and player number
- isCoupPrevented - Checks if any General chose to block a Coup and enforces a cost of 5 coins for doing so.

###Class: Judge
Judge can prevent bribe action, and if been atack with sanction the atacker paies a coin more. Judge inheritor player.
Functions:
- Judge - Constractor get name, role and player number
- isBribePrevented - Checks if any active Judge has chosen to prevent a bribe action. If so, the bribe is blocked.

###Class: GUI
Class for implementing a graphical interface for the game
Functions:
- GUI - Constructor: initializes the window and game state.
- run - Main game loop that processes events and renders the UI.
- handleEvents - Core event handler: processes mouse clicks, keyboard input, and UI interactions.
- render - Draws the current frame based on game state (start or game screen).
- allNamesLocked - Checks if all player names are entered and locked.
- factory - Creates a new Player object based on the given role.
- showWinnerWindow - Displays the victory screen for the winning player.
- showErrorPopup - Shows an error popup message.
- showTemporaryMessage - Displays a temporary text (e.g., coins revealed by a Spy).

Initialization Functions:
- initStartScreen - Sets up the layout and elements of the start screen.
- initPlayerButtons - Initializes player name input buttons and labels.
- initRoleButton - Sets up the button to assign random roles to players.
- initCenterButtons - Initializes main action buttons (e.g., arrest, tax).
- initEnterButton - Initializes the confirm button to complete a turn.


Enjoy the game :)
