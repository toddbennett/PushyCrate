#pragma once

#include "SDL.h"

class PushyCrate {
public:
	PushyCrate(char *filename);
	~PushyCrate();
	int start();
private:
	int width, height, size;
	SDL_Window *window;
	SDL_Surface *surface;
	GameBoard *board;
};