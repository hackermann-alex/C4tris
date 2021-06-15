#include "game.h"
#include "render.h"
#include <stdio.h>

extern SDL_Renderer *renderer;

void gameLoop();

int
main()
{
	uint8_t code = init();
	gameLoop();
	quit(code);
}

void
gameLoop()
{
	SDL_Event e;

	newGame();
	renderBoard();
	renderQueue();
	SDL_RenderPresent(renderer);
start:
	while (SDL_WaitEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return;
		case SDL_KEYDOWN:
			if (e.key.keysym.sym == SDLK_SPACE) {
				genTetromino();
				renderQueue();
				renderPiece();
				SDL_RenderPresent(renderer);
				goto loop;
			}
		}
	}
	goto start;
loop:
	while (SDL_WaitEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return;
		case SDL_KEYDOWN:
			switch (e.key.keysym.sym) {
			case SDLK_UP:
				clearPiece();
				move(0, -1);
				break;
			case SDLK_DOWN:
				clearPiece();
				move(0, 1);
				break;
			case SDLK_LEFT:
				clearPiece();
				move(-1, 0);
				break;
			case SDLK_RIGHT:
				clearPiece();
				move(1, 0);
				break;
			case SDLK_z:
				clearPiece();
				rotate(1);
				break;
			case SDLK_x:
				clearPiece();
				rotate(-1);
				break;
			case SDLK_SPACE:
				depositTetromino();
				genTetromino();
				renderQueue();
			}
			renderPiece();
			SDL_RenderPresent(renderer);
		}
	}
	goto loop;
}
