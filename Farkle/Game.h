#pragma once
#include <vector>
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

public:
	void GameLoop();
};

