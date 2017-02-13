// Gregory S. Fellis
// IT-312 | SNHU | Project 7-1
// 2/9/2017
/* Description:
	This program is an implementation of the dice game, Farkle, as specified by the
	Rules.txt file contained in the project.  Players take turns rolling and 
	collecting dice in order to score points.  The player with the highest score
	after a final round when any player reachers 10000 points is declared the winner.
*/

#include "stdafx.h"
#include "Game.h"

int main()
{
	Game game;

	game.GameLoop();
	
	return 0;
}