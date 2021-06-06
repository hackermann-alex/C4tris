#include "game.h"

game_t game;

void
newGame()
{
	uint8_t i;

	for (i = 0; i < 16; ++i)
		game.currPiece[i] = 0;
	for (i = 0; i < BOARD_H * BOARD_W; ++i)
		game.board[i] = 0;
	for (i = 0; i < 7; ++i)
		game.queue[i] = 0;
	game.hold = 0;
}
