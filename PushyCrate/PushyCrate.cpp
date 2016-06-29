#include "SDL.h"
#include <stdio.h>
#include "GameBoard.h"
#include "PushyCrate.h"

const int maxwidth = 1920;
const int maxheight = 1010;

PushyCrate::PushyCrate(char * filename)
{
	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		printf("Could not open file %s\n", filename);
		return;
	}

	char *boardData = new char[1024];
	int i = 0;
	while (1) {
		int c = fgetc(f);
		if (c == ' ') {
			continue;
		}
		if (c == '\n') {
			width = i;
			break;
		}
		if (c == EOF) {
			printf("Error reading file!\n");
			return;
		}
		boardData[i] = c;
		i++;
	}
	int j = 1;
	while (1) {
		int c;
		for (i = 0; i < width; i++) {
			c = fgetc(f);
			if (c == ' ') {
				continue;
			}
			if (c == '\n' || c == EOF) {
				printf("Error reading file! Wrong width!\n");
				return;
			}
			boardData[j*width + i] = c;
		}
		j++;
		do {
			c = fgetc(f);
		} while (c == '\n' || c == ' ');
		if (c == EOF) {
			height = j;
			break;
		}
		else {
			ungetc(c, f);
		}
	}

	printf("Width is %d\n", width);
	printf("Height is %d\n", height);

	int w = maxwidth / width;
	int h = maxheight / height;
	size = (w < h) ? w : h;
	printf("Size is: %d\n", size);

	window = SDL_CreateWindow("Pushy Crate!", SDL_WINDOWPOS_CENTERED, 30, width*size, height*size, NULL);
	surface = SDL_GetWindowSurface(window);
	board = new GameBoard(width, height, size, boardData);

	fclose(f);
}

PushyCrate::~PushyCrate()
{
	delete board;
	SDL_DestroyWindow(window);
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
				case SDL_SCANCODE_R:
					board->reset();
					break;
				case SDL_SCANCODE_U:
					board->undo();
					break;
				case SDL_SCANCODE_Y:
					board->redo();
					break;
				case SDL_SCANCODE_S:
					board->solve();
					break;
				}
			}
		}
		SDL_FillRect(surface, NULL, 0x00000000);
		board->draw(surface);
		sprintf(title, "Pushy Crate! %d/%d", board->getNumSwitchesPressed(), board->getNumSwitches());
		SDL_SetWindowTitle(window, title);
		SDL_UpdateWindowSurface(window);
	}
	return 0;
}

int main(int argc, char **argv) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Unable to initialize SDL:  %s\n", SDL_GetError());
		return 1;
	}
	PushyCrate *pc = new PushyCrate("C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\soko1.txt");

	pc->start();

	delete pc;
	
	SDL_Quit();
	return 0;
}