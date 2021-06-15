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

uint8_t
move(int8_t x, int8_t y)
{
	const static int8_t bounds[3] = { -1, BOARD_W, BOARD_H };
	short i;

/* Bounds checking */
	for (i = 0; i < 8; i += 2) {
		if (game.currPiece[i] + x == bounds[x > 0] ||
				game.currPiece[i + 1] + y == bounds[(y > 0) * 2])
			return 0;
	}
/* Occupied tile checking */
	for (i = 0; i < 8; i += 2) {
		if (game.board[BOARD_W * (game.currPiece[i + 1] + y) +
				game.currPiece[i] + x])
			return 0;
	}
	for (i = 0; i < 8; i += 2) {
		game.currPiece[i] += x;
		game.currPiece[i + 1] += y;
	}
	return 1;
}

uint8_t
rotate(int8_t rot)
{
/*
	const static int8_t turnTable[8 * 8] = { -1,  0, -1,  1,  0, -2, -1, -2,
						  1,  0,  1, -1,  0,  2,  1,  2, 
						  1,  0,  1, -1,  0,  2,  1,  2,
						 -1,  0, -1,  1,  0, -2, -1, -2,
						  1,  0,  1,  1,  0, -2,  1, -2,
						 -1,  0, -1, -1,  0,  2, -1,  2,
						 -1,  0, -1, -1,  0,  2, -1,  2,
						  1,  0,  1,  1,  0, -2,  1, -2 };
	const static int8_t iTable[8 * 8] = { -2,  0,  1,  0, -2, -1,  1,  2,  
					       2,  0, -1,  0,  2,  1, -1, -2,  
					      -1,  0,  2,  0, -1,  2,  2, -1,  
					       1,  0, -2,  0,  1, -2, -2,  1,  
					       2,  0, -1,  0,  2,  1, -1, -2,  
					      -2,  0,  1,  0, -2, -1,  1,  2,  
					       1,  0, -2,  0,  1, -2, -2,  1,  
					      -1,  0,  2,  0, -1,  2,  2, -1 };
*/
	short i, j, tmp[8];

	if (rot != -1 && rot != 1)
		return 0;
	switch (game.queue[game.head]) {
	case O:
		return 1;
	case I:
		return 1;
	}
	if (!game.currPiece[0] || game.currPiece[0] == BOARD_W - 1 ||
			!game.currPiece[1] || game.currPiece[1] == BOARD_H - 1)
		return 0;
	for (i = 2; i < 8; i += 2) {
		tmp[i] = game.currPiece[i];
		tmp[i + 1] = game.currPiece[i + 1];
		game.currPiece[i] = rot * (game.currPiece[i + 1] - game.currPiece[1]) + game.currPiece[0];
		game.currPiece[i + 1] = rot * (game.currPiece[0] - tmp[i]) + game.currPiece[1];
		if (game.board[BOARD_W * game.currPiece[i + 1] + game.currPiece[i]]) {
			for (j = 2; j <= i + 1; ++j)
				game.currPiece[j] = tmp[j];
			return 0;
		}
	}
	game.currRot = (4 + game.currRot + rot) % 4;
	return 1;
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
	uint8_t piece, i;

	if (game.head == game.tail)
		return;
	game.head = (game.head + 1) % QUEUE_SZ;
	piece = game.queue[game.head] - 1;

	for (i = 0; i < 8; ++i)
		game.currPiece[i] = pieceCord[8 * piece + i];
	for (i = 0; i < 8; i += 2)
		game.currPiece[i] += 3;
	game.currRot = 0;
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
