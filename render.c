#include "game.h"
#include "render.h"

#define NAME "Tetris"
#define BG 0x10, 0x1A, 0x1D, 0xFF 
#define GRID_COLOUR 0xE0, 0xE0, 0xE0

#define MIN(a, b) ((a) < (b) ? (a) : (b))

enum codes { SUCCESS, INIT_SDL, RENDERER, WINDOW };
const static uint8_t colours[3 * 8] = { 0x80, 0x80, 0x80,
					0x00, 0xFF, 0xFF,
					0x00, 0x00, 0xFF,
					0xFF, 0xAA, 0x00,
					0xFF, 0xFF, 0x00,
					0x00, 0xFF, 0x00,
					0x99, 0x00, 0xFF,
					0xFF, 0x00, 0x00 };

extern game_t game;

SDL_Renderer *renderer;
static SDL_Window *window;
static uint16_t size;
static uint16_t originX;
static uint16_t originY;

void
renderTile(uint8_t row, uint8_t col)
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
renderPreview(short x, short y, uint8_t piece)
{
	short i;
	SDL_Rect renderQuad = { 0, 0, size, size };

	if (!piece)
		return;
	SDL_SetRenderDrawColor(renderer, colours[3 * piece],
			colours[3 * piece + 1], colours[3 * piece + 2], 0xFF);

	--piece;
	for (i = 0; i < 8; i += 2) {
		renderQuad.x = x + size * pieceCord[8 * piece + i];
		renderQuad.y = y + size * pieceCord[8 * piece + i + 1];
		SDL_RenderFillRect(renderer, &renderQuad);
	}
}

void
renderPiece()
{
	short i;
	SDL_Rect renderQuad = { 0, 0, size, size };

	if (!game.queue[game.head])
		return;
	for (i = 0; i < 8; ++i) {
		renderQuad.x = game.currPiece[i];
		renderQuad.y = game.currPiece[++i];
		SDL_SetRenderDrawColor(renderer,
				colours[game.queue[game.head]],
				colours[game.queue[game.head] + 1],
				colours[game.queue[game.head] + 2], 0xFF);
		SDL_RenderDrawRect(renderer, &renderQuad);
	}
}

void
renderBoard()
{
	short i, j;

	for (i = 0; i < BOARD_H; ++i) {
		for (j = 0; j < BOARD_W; ++j)
			renderTile(i, j);
	}
}

void
renderQueue()
{
	short i, x = originX + size * (GAP + BOARD_W), y = originY;

	for (i = 1; i <= PREVIEW; ++i) {
		renderPreview(x, y, QUEUE_ELEM(i));
		y += 4 * size;
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
