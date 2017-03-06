// Game.cpp
/* Description:
	This file makes up the bulk of the Farkle game.  It will guide the players
	through player creation, rolling dice, taking turns, scoring and declaring
	the winner of the game.
*/

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <array>

#include "Player.h"
#include "Game.h"
#include "Menu.h"

using namespace std;

#pragma region Inputs
// Clears the input stream when invalid characters are used
void Game::ClearInput()
{
	// if an invalid number is detected, clear the input stream
	cin.clear();

	// remove characters from the input stream until a newline or EOF is met			
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Returns an Integer for player input
int Game::GetInput()
{
	unsigned int sel;

	// keep repeating until a valid number is stored in sel
	while (!(cin >> sel))
	{
		ClearInput();

		cout << "Invalid selection. Try again: ";
	}

	ClearInput();

	return sel;
}
#pragma endregion

#pragma region Messages
// Congratulate the winner
void Game::Congratulate(Player& player)
{
	system("cls");
	cout << "=================" << endl;
	cout << "CONGRATULATIONS" << endl;
	cout << "=================" << endl;
	cout << player.Name() << " is the winner with a score of " << player.Score() << "!" << endl << endl;

	system("pause");
}

// Alert the players the final round has begun
void Game::FinalRound(Player& player)
{
	system("cls");
	cout << "=================" << endl;
	cout << "FINAL ROUND" << endl;
	cout << "=================" << endl;
	cout << player.Name() << " has a score of " << player.Score() << "!" << endl; 
	cout << "The final round of play has begun! Good luck!" << endl << endl;

	system("pause");
}

// Display dice kept this round
void Game::ShowKeptDice(array<int, 6>& keptDice)
{
	cout << "+------------+" << endl;
	cout << "| Kept Dice  |" << endl;
	cout << "+------------+" << endl;

	for (unsigned int i = 0; i < keptDice.size(); i++)
	{
		cout << "[" << (i + 1) << "]: " << keptDice[i];

		if (i != keptDice.size() - 1)
		{
			cout << ", ";
		}
	}

	cout << endl << endl;
}

// Display current dice pool values
void Game::ShowRolls(vector<int>& dicePool)
{
	cout << "+------------+" << endl;
	cout << "| Dice Rolls |" << endl;
	cout << "+------------+" << endl;

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		cout << "| Die " << (i + 1) << ": [" << dicePool[i] << "] |" << endl;
	}

	cout << "+------------+" << endl << endl;
}

// Display welcome message to players
void Game::WelcomePlayers(vector<Player>& players)
{
	string playerNames;

	system("cls");
	cout << "=================" << endl;
	cout << "WELCOME" << endl;
	cout << "=================" << endl;

	// setup playerNames for Welcome Message
	for (unsigned int i = 0; i < players.size(); i++)
	{
		if (i == players.size() - 1)
		{
			playerNames += "and " + players[i].Name();
		}
		else
		{
			playerNames += players[i].Name() + ", ";
		}
	}

	cout << "Welcome " << playerNames << "!" << endl << endl;

	system("pause"); // Wait for player to press enter
}
#pragma endregion

#pragma region Setup
// Prompt the user for the Number of Players
void Game::PromptNumPlayers()
{
	system("cls");
	cout << "=================" << endl;
	cout << "NEW GAME" << endl;
	cout << "=================" << endl;
	cout << "How many players are there? ";
}

// Setup player names
void Game::SetupPlayerNames(vector<Player>& players)
{
	system("cls");
	cout << "=================" << endl;
	cout << "PLAYER NAMES" << endl;
	cout << "=================" << endl;
	
	// Get the name for each player in the players vector
	for (unsigned int i = 0; i < players.size(); i++)
	{
		string name;
		
		cout << "Enter name for Player " << (i + 1) << " (no spaces): ";
		
		cin >> name;

		ClearInput();

		// create a new player object in the player vector using the name provided
		players[i] = Player(name);	
	}
}

#pragma endregion

#pragma region Play
// Counts different dice values in a roll
array<int, 6> Game::CountDice(vector<int>& dicePool)
{
	// Create new array and set all values to 0
	array<int, 6> counter = { 0 };

	// Count each die for it's value and add that to the counter array
	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		counter[dicePool[i] - 1]++;
	}

	return counter;
}

// Check if a player is over 10,000
bool Game::CheckFor10k(int score)
{
	bool isOver10k = score >= 10000 ? true : false;

	return isOver10k;
}

// Check if the dice roll has any scoring dice
bool Game::ScoringDice(array<int, 6>& diceCount)
{
	// If there is a 1 or a 5, return true
	if (diceCount[0] >= 1 || diceCount[4] >= 1)
	{
		return true;
	}
	
	// If no 1 or 5, check for any triples
	for (unsigned int i = 1; i < diceCount.size(); i++)
	{
		if (i == 4) { i = 5; }

		if (diceCount[i] >= 3) { return true; }
	}
	
	// No scoring dice
	return false;
}

// Checks if the selection is a valid die in the dice pool
bool Game::IsValid(unsigned int& selection, vector<int>& dicePool)
{
	if (selection > 0 && selection <= dicePool.size())
	{
		return true;
	}

	return false;
}

// Scores and removes the dice selected by the player
int Game::ScoreDice(vector<int>& dicePool, int die, array<int, 6>& dieCount, array<int, 6>& keptDice)
{
	die -= 1;	// because of 0 indexing, reduce the selected die number by 1
	int score = 0;
	int dieValue = dicePool[die];

	// If there are 3 or more dice with the selected value, set aside 3 of the dice
	if (dieCount[dieValue - 1] >= 3)
	{
		int i = 0;
		while (i < 3)
		{
			for (unsigned int j = 0; j < dicePool.size(); j++)
			{
				if (dicePool[j] == dieValue)
				{					
					dicePool.erase(dicePool.begin() + j); // Remove the selected die from the dicepool
					keptDice[dieValue - 1]++; // Increment the dice kept for this turn
					i++;
				}
			}
		}

		// if 3 1's were kept, the score is 1000, otherwise is 100x the die value
		score = dieValue == 1 ? 1000 : dieValue * 100;
	}
	else if (dieValue == 1 || dieValue == 5)
	{			
		dicePool.erase(dicePool.begin() + die);	// Remove selected die from the dicepool
		keptDice[dieValue - 1]++; // Incredment the dice kept for this turn

		// If die is 1, score is 100, otherwise it's 50
		score = dieValue == 1 ? 100 : 50;
	}

	return score;
}

// A method to roll all the dice in a provided pool
void Game::RollDice(vector<int>& dicePool)
{
	random_device rng;

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		dicePool[i] = rng() % 6 + 1; // Creates a random number from 1 to 6
	}
}

// Get the name of the player with the highest score
Player Game::GetWinner(vector<Player> players)
{
	Player winner = players[0];

	for (unsigned int i = 1; i < players.size(); i++)
	{
		if (players[i].Score() > players[i - 1].Score())
		{
			winner = players[i];
		}
	}

	return winner;
}
#pragma endregion

#pragma region Private Loops
// Primary Game Loop
Player Game::Play()
{
	Player winner; // While this string is empty, the game will continue to loop
	int numOfPlayers = 0; // Int to hold the number of players
	int playerTurn = 0; // Int that keeps track of which players turn it is, based on vector index
	int firstTo10k = -1; // Int to mark the first players turn that has hit 10k
	int turnScore = 0; // The value the player has scored this turn

	PromptNumPlayers();

	// Loop until players is at least 2
	while (!(numOfPlayers > 1))
	{
		numOfPlayers = GetInput();

		if (numOfPlayers < 2)
		{
			cout << "Invalid number of players. Must be more than 1." << endl;
			cout << "Try again: ";
		}
	}

	vector<Player> players(numOfPlayers); // generate a vector to hold Player objects based on number of players

	SetupPlayerNames(players);

	WelcomePlayers(players);

	// Continue looping until a winner is declared
	while (winner.Name() == "") 
	{
		// Return to first players turn if necessary
		if (playerTurn == players.size())
		{
			playerTurn = 0;
		}

		// The game is over if all players have had a turn after a player reaches 10k
		if (firstTo10k >= 0 && playerTurn == firstTo10k)
		{
			winner = GetWinner(players);
			continue;
		}

		// Run a turn for a player and put their score into the turnScore variable
		turnScore = Turn(players[playerTurn]);

		// If the player has not entered the game and they score more than 1000 points
		if (!players[playerTurn].InGame() && turnScore >= 1000)
		{
			players[playerTurn].SetInGame(); // put player in the game
		}

		// add turnscore to players score
		if (turnScore > 0)
		{
			players[playerTurn].AddScore(turnScore);
		}

		// Check if players score is over 10k
		if (firstTo10k == -1 && CheckFor10k(players[playerTurn].Score()))
		{
			FinalRound(players[playerTurn]);

			firstTo10k = playerTurn;
		}

		playerTurn++; // Send to next players turn
	}

	return winner;
}

// Turn loop
int Game::Turn(Player& player)
{
	vector<int> dicePool(6); // New dice pool of 6 dice
	array<int, 6> keptDice = { 0 }; // Set all kept dice values to 0
	array<int, 6> diceCount = { 0 }; // Variable to count die values
	int turnScore = 0; // Running score for this turn
	bool farkle = false; // Determines if player has Farkled
	bool canPass = player.InGame(); // Determines if player can pass their turn
	bool canReroll = false; // Determines if player can reroll the dice
	bool hasPassed = false; // Determines when the player has passed their turn

	unsigned int selection = 1;

	while (!hasPassed)
	{
		// if player has set aside all dice
		if (dicePool.size() == 0)
		{
			// populate dicePool back to 6 dice
			for (int i = 0; i < 6; i++)
			{
				dicePool.push_back(1);
			}
		}
		
		RollDice(dicePool);
		canReroll = false;
		canPass = false;

		// Allow player to set aside the rolled dice until they pass or reroll
		do
		{
			system("cls");
			cout << "=================" << endl;
			cout << player.Name() << "'s Turn!" << endl;
			cout << "=================" << endl;
			cout << player.Name() << "'s Score: " << player.Score() << endl;
			cout << "Turn Score: " << turnScore << endl << endl;

			ShowRolls(dicePool);
			ShowKeptDice(keptDice);

			diceCount = CountDice(dicePool);

			// If the player has no scoring dice and they can't reroll
			if (!ScoringDice(diceCount) && !canReroll)
			{				
				farkle = true; // The player has farkled
				selection = 0; // Automatically select to end their turn
				continue; // Skip the rest of the code in this loop
			}	

			cout << "Enter the die # you wish to keep. Triple values will automatically be kept." << endl;
			cout << "Enter 9 to reroll dice or 0 to end your turn." << endl;
			cout << "Selection: ";

			// If the dice pool is empty, automatically reroll to generate 6 new dice, otherwise get the player's selection
			selection = dicePool.size() == 0 ? 9 : GetInput();

			if (IsValid(selection, dicePool))
			{
				// Add the dice value to turn score
				turnScore += ScoreDice(dicePool, selection, diceCount, keptDice);
							
				canReroll = true; // Player has kept a die and can now reroll the dice
				
				// If the player is in the game or they have over 1000 points, the player can end their turn
				canPass = (player.InGame() || turnScore >= 1000) ? true : false;

			}			
			// check if the player has selected a valid move, ending turn or rerolling dice
			else if ((selection == 0 && !canPass) || (selection == 9 && !canReroll))
			{
				string msg;

				system("cls");
				cout << "=================" << endl;
				cout << "INVALID MOVE" << endl;
				cout << "=================" << endl;

				msg = selection == 0 ? player.Name() + " cannot score out at this time!" : player.Name() + " cannot reroll at this time!";

				cout << msg << endl << endl;

				selection = 7;

				system("pause");
			}
		} while (selection > 0 && selection != 9);

		// Player selected to end their turn, then they have passed. Otherwise they're rolling dice
		hasPassed = selection == 0 ? true : false;
	}

	system("cls");
	cout << "=================" << endl;
	cout << player.Name() << "'s TURN ENDS!" << endl;
	cout << "=================" << endl;

	if (farkle)
	{
		cout << player.Name() << " has FARKLED!" << endl;
	}
	else
	{
		cout << player.Name() << " has scored " << turnScore << " points this turn!" << endl;
	}

	system("pause");

	// return 0 if player farkled or turnScore
	return turnScore = farkle ? 0 : turnScore;
}
#pragma endregion

void Game::GameLoop()
{
	bool quit = false;
	unsigned int selection;
	Player winner;

	while (!quit)
	{
		Menu::ShowMenu();		
		selection = GetInput();

		switch (selection)
		{
		case 1:	// New Game
			winner = Play();
			Congratulate(winner);
			break;
		case 2:	// Show Rules
			Menu::ShowRules();
			break;
		case 3:	// Quit
			quit = true;
			break;
		default: // Anything else
			break;
		}		
	}	
}