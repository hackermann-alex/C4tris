#include "game.h"
#include "render.h"

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
loop:
	while (SDL_WaitEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			return;
		}
	}
	goto loop;
}
