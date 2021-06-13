#include "game.h"
#include <stdlib.h>
#include <time.h>

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
	const static uint8_t bounds[4] = { 0, BOARD_H - 1, 0, BOARD_W - 1 };
	short i, start = !(dir >> 1), diff = ((dir & 1) << 1) - 1;

	for (i = start; i < 8; i += 2) {
		if (game.currPiece[i] == bounds[dir])
			return;
	}
	for (i = start; i < 8; i += 2)
		game.currPiece[i] += diff;
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
depositTetromino()
{
	short i;

	for (i = 0; i < 8; i += 2)
		game.board[BOARD_W * game.currPiece[i + 1] + game.currPiece[i]]
			= game.queue[game.head];
}

void
genTetromino()
{
	uint8_t piece;
	short i;

	if (game.head == game.tail)
		return;
	game.head = (game.head + 1) % QUEUE_SZ;
	piece = game.queue[game.head] - 1;

	for (i = 0; i < 8; ++i)
		game.currPiece[i] = pieceCord[8 * piece + i];
	for (i = 0; i < 8; i += 2)
		game.currPiece[i] += 3;
	genPieces();
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
