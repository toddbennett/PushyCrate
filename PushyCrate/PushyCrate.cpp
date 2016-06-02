#include "SDL.h"
#include <stdio.h>
#include "GameBoard.h"
#include "PushyCrate.h"


// TODO: refactor GameBoard to not need these to be exactly 10
const int maxwidth = 1280;
const int maxheight = 650;

PushyCrate::PushyCrate(char * filename)
{
	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		printf("Could not open file %s\n", filename);
		return;
	}
	if (fscanf(f, "%d,%d\n\n", &width, &height) != 2) {
		printf("Could not read file %s\n", filename);
		return;
	}
	printf("Width is %d\n", width);
	printf("Height is %d\n", height);

	int w = maxwidth / width;
	int h = maxheight / height;
	size = (w < h) ? w : h;
	printf("Size is: %d\n", size);

	char *boardData = new char[width*height];
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			boardData[i + j*width] = fgetc(f);
		}
		fgetc(f);
	}

	window = SDL_CreateWindow("Pushy Crate!", SDL_WINDOWPOS_CENTERED, 30, width*size, height*size, NULL);
	surface = SDL_GetWindowSurface(window);
	board = new GameBoard(width, height, size, boardData);
}

PushyCrate::~PushyCrate()
{
}

int PushyCrate::start()
{
	int keepGoing = 1;
	while (keepGoing) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				keepGoing = 0;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_RIGHT:
					board->movePlayer(dir_right);
					break;
				case SDL_SCANCODE_UP:
					board->movePlayer(dir_up);
					break;
				case SDL_SCANCODE_LEFT:
					board->movePlayer(dir_left);
					break;
				case SDL_SCANCODE_DOWN:
					board->movePlayer(dir_down);
					break;
				}
			}
		}
		SDL_FillRect(surface, NULL, 0xFFFFFFFF);
		board->draw(surface);
		SDL_UpdateWindowSurface(window);
	}
	return 0;
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Unable to initialize SDL:  %s\n", SDL_GetError());
		return 1;
	}

	PushyCrate *pc = new PushyCrate("C:\\Users\\xtodd\\Desktop\\soko1.txt");

	pc->start();
	
	SDL_Quit();
	return 0;
}