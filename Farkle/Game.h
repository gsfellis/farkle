#pragma once
#include <vector>
#include <array>
#include "Player.h"

class Game
{
private:	
	// Inputs
	void ClearInput();
	int GetInput();
	
	// Messages
	void Congratulate(Player& player);
	void FinalRound(Player& player);
	void ShowKeptDice(array<int, 6>& keptDice);
	void ShowRolls(vector<int>& dicePool);
	void WelcomePlayers(vector<Player>& players);
	
	// Setup
	void PromptNumPlayers();
	void SetupPlayerNames(vector<Player>& players);	
	
	// Play
	array<int, 6> CountDice(vector<int>& dicePool);
	bool CheckFor10k(int score);	
	bool IsValid(unsigned int& selection, vector<int>& dicePool);
	bool ScoringDice(array<int, 6>& diceCount);
	int ScoreDice(vector<int>& dicePool, int die, array<int, 6>& diceCount, array<int, 6>& keptDice);
	void RollDice(vector<int>& dicePool);
	Player GetWinner(vector<Player> players);

	// Private Loops
	int Turn(Player& player);
	Player Play();

public:
	void GameLoop();
};