#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define BOARD_H 20
#define BOARD_W 10
#define PREVIEW 5
#define QUEUE_SZ (PREVIEW + 7)

#define QUEUE_ELEM(i) game.queue[(game.head + (i)) % QUEUE_SZ]

enum tetrominos { NONE, I, J, L, O, S, T, Z };

const static uint8_t pieceCord[7 * 8] = { 0, 1, 1, 1, 2, 1, 3, 1,
					  1, 1, 2, 1, 0, 0, 0, 1,
					  1, 1, 2, 1, 2, 0, 0, 1,
					  1, 0, 2, 0, 1, 1, 2, 1,
					  1, 1, 2, 0, 1, 0, 0, 1,
					  1, 1, 2, 1, 1, 0, 0, 1,
					  1, 1, 2, 1, 1, 0, 0, 0 };

typedef struct {
	uint8_t currPiece[8];
	uint8_t currRot;
	uint8_t board[BOARD_H * BOARD_W];
	uint8_t queue[QUEUE_SZ];
	int8_t head;
	int8_t tail;
	uint8_t hold;
} game_t;

void newGame();
uint8_t move(int8_t x, int8_t y);
uint8_t rotate(int8_t rot);
void depositTetromino();
void genTetromino();

#endif
