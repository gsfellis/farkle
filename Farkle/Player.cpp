#include "stdafx.h"
#include <string>
#include "Player.h"

using namespace std;
Player::Player()
{
}

// New player constructor
Player::Player(string n) : name(n)
{
}

// Show score
int Player::Score()
{
	return score;
}

// Add Score
void Player::AddScore(int value)
{
	score += value;
}

// Show name
string Player::Name()
{
	return name;
}