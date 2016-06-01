#include "SDL.h"
#include <stdio.h>
#include "GameBoard.h"


// TODO: refactor GameBoard to not need these to be exactly 10
const int width = 10;
const int height = 10;

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Unable to initialize SDL:  %s\n", SDL_GetError());
		return 1;
	}
	
	SDL_Window *window = SDL_CreateWindow("Pushy Crate!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width*64, height*64, NULL);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	GameBoard *board = new GameBoard();

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
	
	SDL_Quit();
	return 0;
}
