#ifndef GAME_H
#define GAME_H

#include <stdint.h>

#define BOARD_H 20
#define BOARD_W 10

enum tetrominos { NONE, I, J, L, O, S, T, Z };

typedef struct {
	uint8_t currPiece[16];
	uint8_t board[BOARD_H * BOARD_W];
	uint8_t queue[7];
	uint8_t hold;
} game_t;

void newGame();

#endif
