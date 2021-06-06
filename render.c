#include "game.h"
#include "render.h"

#define NAME "Tetris"
#define BG 0x47, 0x80, 0x61, 0xFF 
#define GRID_COLOUR 0xE0, 0xE0, 0xE0

#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum codes { SUCCESS, INIT_SDL, RENDERER, WINDOW };
const static uint8_t colours[3 * 8] = { 0x80, 0x80, 0x80,
					0x00, 0xFF, 0xFF,
					0x00, 0x00, 0xFF,
					0xFF, 0x80, 0x00,
					0xFF, 0xFF, 0x00,
					0x00, 0xFF, 0x00,
					0xFF, 0x00, 0xFF,
					0xFF, 0x00, 0x00 };

extern game_t game;

SDL_Renderer *renderer;
static SDL_Window *window;
static uint16_t size;
static uint16_t originX;
static uint16_t originY;

void
renderTile(uint8_t row, uint8_t col)
/* Consider moving colours to a static array (eliminating switch) */
{
	SDL_Rect renderQuad = { originX + size * col,
			 originY + size * row, size,  size };
	uint8_t i = 3 * game.board[BOARD_W * row + col];

	SDL_SetRenderDrawColor(renderer,
			colours[i], colours[i + 1], colours[i + 2], 0xFF);
	SDL_RenderFillRect(renderer, &renderQuad);

	if (!i) {
		renderQuad.x += 1;
		renderQuad.y += 1;
		renderQuad.w -= 2;
		renderQuad.h -= 2;
		SDL_SetRenderDrawColor(renderer, GRID_COLOUR, 0xFF);
		SDL_RenderDrawRect(renderer, &renderQuad);
	}
}

void
renderScene()
{
	uint8_t i, j;

	for (i = 0; i < BOARD_H; ++i) {
		for (j = 0; j < BOARD_W; ++j)
			renderTile(i, j);
	}
}

uint8_t
init()
{
	int w, h;
	uint8_t scaleX, scaleY;

	/* Init SDL */
	if (SDL_Init(SDL_INIT_VIDEO))
		return INIT_SDL;
	/* Create window */
	window = SDL_CreateWindow(NAME,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
			SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window)
		return WINDOW;
	/* Create renderer */
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
		return RENDERER;
	/* Render scene */
	SDL_SetRenderDrawColor(renderer, BG);
	SDL_RenderClear(renderer);

	SDL_GetWindowSize(window, &w, &h);
	scaleX = w / BOARD_W >> 1;
	scaleY = h / BOARD_H >> 1;
	size = MIN(scaleX, scaleY);
	originX = (w - size * BOARD_W) >> 1;
	originY = (h - size * BOARD_H) >> 1;
	return SUCCESS;
}

void
quit(uint8_t code)
{
	switch (code) {
	case SUCCESS:
		SDL_DestroyRenderer(renderer);
	case RENDERER:
		SDL_DestroyWindow(window);
	case WINDOW:
		SDL_Quit();
	default:
		exit(code);
	}
}
