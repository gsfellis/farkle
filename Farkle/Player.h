#pragma once
#include "stdafx.h"
#include <string>

using namespace std;

class Player {
private:
	string name;
	int score;
	bool inGame;

public:
	Player();
	Player(string name); // New player constructor

	string Name(); // show Name
	
	void AddScore(int value); // add to score
	int Score(); // show score	
	
	void SetInGame(); // set inGame	
	bool InGame();	// return inGame status
};