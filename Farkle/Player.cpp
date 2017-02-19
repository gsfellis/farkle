// Player.cpp
/* Description:
	The file is the Player class.  The player class is an object that
	represents each player in the Farkle game.  It holds each player's
	score, name and in game status.  It also has methods for returning
	each of these, setting the in game status and adding points to the 
	player's score at the end of each turn.	
*/

#include "stdafx.h"
#include <string>
#include "Player.h"

using namespace std;
Player::Player()
{
}

// New player constructor
Player::Player(string n) : name(n), score(0), inGame(false)
{
}

// Show name
string Player::Name()
{
	return name;
}

// Add to Score
void Player::AddScore(int value)
{
	score += value;
}

// Show score
int Player::Score()
{
	return score;
}

// set inGame
void Player::SetInGame()
{
	inGame = true;
}

// return inGame status
bool Player::InGame()
{
	return inGame;
}