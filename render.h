#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>

#define GAP 1

uint8_t init();
void renderBoard();
void renderQueue();
void quit(uint8_t code);

#endif
