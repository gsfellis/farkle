#pragma once
#include "stdafx.h"
#include <string>

using namespace std;

class Player {
private:
	string name = "";
	int score = 0;

public:
	Player();

	// New player constructor
	Player(string name);

	// show score
	int Score();

	// add score
	void AddScore(int value);

	// show Name
	string Name();
};
