#pragma once
#include <vector>
#include <array>
#include "Player.h"

class Game
{
private:	
	int GetInput();
	string Play();
	void ShowNumPlayers();
	void SetupPlayerNames(vector<Player>& players);
	int Turn(Player player);
	bool CheckFor10k(int score);
	string GetWinner(vector<Player> players);
	void Congratulate(string player);
	void WelcomePlayers(vector<Player>& players);
	
	void RollDice(vector<int>& dicePool);
	void ShowRolls(vector<int>& dicePool);
	array<int, 6> CountDice(vector<int>& dicePool);
	bool IsValid(unsigned int& selection, vector<int>& dicePool);

public:
	void GameLoop();
};

