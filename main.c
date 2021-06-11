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
	genTetromino();
	renderPiece();
	renderQueue();
	SDL_RenderPresent(renderer);
loop:
	while (SDL_WaitEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return;
		case SDL_KEYDOWN:
			clearPiece();
			switch (e.key.keysym.sym) {
			case SDLK_UP:
				move(UP);
				break;
			case SDLK_DOWN:
				move(DOWN);
				break;
			case SDLK_LEFT:
				move(LEFT);
				break;
			case SDLK_RIGHT:
				move(RIGHT);
				break;
			case SDLK_SPACE:
				genTetromino();
				renderQueue();
			}
			renderPiece();
			SDL_RenderPresent(renderer);
		}
	}
	goto loop;
}
