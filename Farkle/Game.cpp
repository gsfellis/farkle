#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <array>

#include "Player.h"
#include "Game.h"
#include "Menu.h"

using namespace std;

int Game::GetInput()
{
	unsigned int sel;

	while (!(cin >> sel))
	{
		// if an invalid number is detected, clear the input stream
		cin.clear();

		// remove characters from the input stream until a newline or EOF is met			
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid selection. Try again: ";
	}

	return sel;
}

void Game::ShowNumPlayers()
{
	system("cls");
	cout << "=================" << endl;
	cout << "NEW GAME" << endl;
	cout << "=================" << endl << endl;
	cout << "How many players are there? ";
}

// Setup player names
void Game::SetupPlayerNames(vector<Player>& players)
{
	system("cls");
	cout << "=================" << endl;
	cout << "PLAYER NAMES" << endl;
	cout << "=================" << endl << endl;
	
	for (unsigned int i = 0; i < players.size(); i++)
	{
		string name;		
		
		cout << "Enter name for Player " << (i + 1) << ": ";
		cin >> name;

		players[i] = Player(name);
	}
}

// Primary Game Loop
string Game::Play()
{
	string winner = "";
	//bool noWinner = true;
	int numOfPlayers = 0;
	int playerTurn = 0;
	int firstTo10k = -1;
	int turnScore = 0;
	
	ShowNumPlayers();
	while (!(numOfPlayers > 1))
	{
		numOfPlayers = GetInput();

		if (numOfPlayers < 2)
		{
			cout << "Invalid number of players. Must be more than 1." << endl;
			cout << "Try again: ";
		}
	}

	vector<Player> players(numOfPlayers); // vector to hold player objects
	
	SetupPlayerNames(players);

	WelcomePlayers(players);

	while (winner == "")
	{
		// Return to first players turn
		if (playerTurn == players.size())
		{
			playerTurn = 0;
		}

		// Check if we're at the first player to get 10k
		if (firstTo10k >= 0 && playerTurn == firstTo10k)
		{
			winner = GetWinner(players);			
		}

		// Run a turn for a player
		turnScore = Turn(players[playerTurn]);

		if (!players[playerTurn].InGame() && turnScore >= 1000)
		{
			players[playerTurn].SetInGame();
		}

		// add turnscore to players score
		players[playerTurn].AddScore(turnScore);

		// Check if players score is over 10k
		if (firstTo10k == -1 && CheckFor10k(players[playerTurn].Score()))
		{
			firstTo10k = playerTurn;
		}		

		playerTurn++; // Send to next players turn
	}

	return winner;
}

// Turn loop
int Game::Turn(Player& player)
{
	vector<int> dicePool(6);
	//vector<int> keepDice;
	array<int, 6> diceCount = { 0 };
	int turnScore = 0;
	bool farkle = true,
		canPass = player.InGame(),
		canReroll = false,
		hasPassed = false,
		reroll = false;
		
	unsigned int selection = 1;

	while (!hasPassed)
	{
		if (dicePool.size() == 0)
		{
			// populate dicePool back to 6 dice
			for (int i = 0; i < 6; i++)
			{
				dicePool.push_back(1);
			}
		}
		
		RollDice(dicePool);
		farkle = true;
		canReroll = false;
		
		do
		{
			system("cls");
			cout << "=================" << endl;
			cout << player.Name() << "'s Turn!" << endl;
			cout << "=================" << endl;
			cout << player.Name() << "'s Score: " << player.Score() << endl;
			cout << "Turn Score: " << turnScore << endl << endl;

			ShowRolls(dicePool);

			diceCount = CountDice(dicePool);

			cout << "Enter the die # you wish to keep. Triple values will automatically be kept." << endl;
			cout << "Enter 9 to reroll dice or 0 to end your turn." << endl;
			cout << "Selection: ";

			selection = GetInput();

			if (IsValid(selection, dicePool))
			{
				turnScore += ScoreDice(dicePool, selection, diceCount);
								
				farkle = false;	
				canReroll = true;
				canPass = (player.InGame() || turnScore >= 1000) ? true : false;

				cout << "canPass: " << boolalpha << canPass << endl;

				system("pause");
			}			
			
			if (selection == 0 && !farkle)
			{
				if ((selection == 0 && !canPass) || (selection == 9 && !canReroll))
				{
					string msg;

					system("cls");
					cout << "=================" << endl;
					cout << "INVALID MOVE" << endl;
					cout << "=================" << endl;

					msg = selection == 0 ? player.Name() + " has not entered the game and has not scored 1000 points this turn!" : player.Name() + " cannot reroll at this time!";

					cout << msg << endl << endl;

					selection = 1;

					system("pause");
				}
			}
		} while (selection > 0 && selection != 9);

		if (selection == 0)
		{
			hasPassed = true;
			system("pause");
		}	
	}

	// return 0 if player farkled or turnScore
	return turnScore = farkle ? 0 : turnScore;	
}

int Game::ScoreDice(vector<int>& dicePool, int die, array<int, 6>& dieCount)
{
	die -= 1;
	int score = 0;
	int dieValue = dicePool[die];

	if (dieCount[dieValue - 1] >= 3)
	{
		int i = 0;
		while (i < 3)
		{
			for (int j = 0; j < dicePool.size(); j++)
			{
				if (dicePool[j] == dieValue)
				{
					//keepDice.push_back(dicePool[j]);
					RemoveFromDicePool(dicePool, j);
					i++;
				}
			}
		}

		score = dieValue == 1 ? 1000 : dieValue * 100;
	}
	else if (dieValue == 1 || dieValue == 5)
	{
		//keepDice.push_back(dicePool[die]);
		RemoveFromDicePool(dicePool, die);

		score = dieValue == 1 ? 100 : 50;
	}

	return score;
}

void Game::RemoveFromDicePool(vector<int>& dicePool, int& die)
{
	dicePool.erase(dicePool.begin() + die);
}

bool Game::IsValid(unsigned int& selection, vector<int>& dicePool)
{
	if (selection > 0 && selection <= dicePool.size())
	{
		return true;
	}

	return false;
}

// Counts different dice values in a roll
array<int, 6> Game::CountDice(vector<int>& dicePool)
{
	array<int, 6> counter = { 0 };

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		counter[dicePool[i] - 1]++;
	}

	return counter;
}

// A method to show dice rolls
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

// A method to roll all the dice in a provided pool
void Game::RollDice(vector<int>& dicePool)
{
	random_device rng;

	for (unsigned int i = 0; i < dicePool.size(); i++)
	{
		dicePool[i] = rng() % 6 + 1;
	}
}

// Display welcome message to players
void Game::WelcomePlayers(vector<Player>& players)
{
	string playerNames;
	
	system("cls");
	cout << "=================" << endl;
	cout << "WELCOME" << endl;
	cout << "=================" << endl << endl;

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

	system("pause");
}

// Check if a player is over 10,000
bool Game::CheckFor10k(int score)
{
	bool check = score >= 10000 ? true : false;

	return check;
}

// Get the name of the player with the highest score
string Game::GetWinner(vector<Player> players)
{
	string winner = players[0].Name();

	for (unsigned int i = 1; i < players.size(); i++)
	{
		winner = players[i].Score() > players[i - 1].Score() ? players[i].Name() : winner;
	}

	return winner;
}

// Congratulate the winner
void Game::Congratulate(string winner)
{
	system("cls");
	cout << "=================" << endl;
	cout << "CONGRATULATIONS" << endl;
	cout << "=================" << endl << endl;
	cout << winner << " is the winner!" << endl << endl;

	system("pause");
}

void Game::GameLoop()
{
	bool quit = false;
	unsigned int selection;
	string winner;

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