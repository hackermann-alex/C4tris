#include "game.h"
#include <stdlib.h>
#include <time.h>

enum dir { UP, DOWN, LEFT, RIGHT };

game_t game;

inline uint8_t
queueLen()
{
	return game.tail < 0 ? 0 :
		game.tail - game.head + (game.head > game.tail) * QUEUE_SZ + 1;
}

void
move(uint8_t dir)
{
	short i, diff = ((dir & 1) << 1) - 1;

	for (i = !(dir >> 1); i < 8; i += 2)
		game.currPiece[i] += diff;
}

void
genTetromino()
{
	uint8_t piece;
	short i;

	if (game.head == game.tail)
		return;
	piece = game.queue[game.head] - 1;
	game.head = (game.head + 1) % QUEUE_SZ;

	for (i = 0; i < 8; ++i)
		game.currPiece[i] = pieceCord[8 * piece + i];
}

void
genPieces()
{
	uint8_t tmp, randIdx;
	short i, start;
	
	if (queueLen() > QUEUE_SZ - 7)
		return;
	start = game.tail < 0 ? game.head + !game.head * QUEUE_SZ : game.tail;
	game.tail = (start + 7) % QUEUE_SZ;
	for (i = 1; i < 8; ++i)
		game.queue[(start + i) % QUEUE_SZ] = i;
	for (i = 7; i > 1; --i) {
		randIdx = (start + (rand() % i) + 1) % QUEUE_SZ;
		tmp = game.queue[(start + i) % QUEUE_SZ];
		game.queue[(start + i) % QUEUE_SZ] = game.queue[randIdx];
		game.queue[randIdx] = tmp;
	}
}

void
newGame()
{
	short i;

	srand(time(NULL));
	for (i = 0; i < BOARD_H * BOARD_W; ++i)
		game.board[i] = NONE;
	game.hold = NONE;
	game.head = 0;
	game.tail = -1;
	genPieces();
}
