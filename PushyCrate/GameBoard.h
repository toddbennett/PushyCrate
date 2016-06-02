#pragma once
#include "GamePiece.h"

enum Dir { dir_right, dir_up, dir_left, dir_down };

class GameBoard
{
public:
	GameBoard(int width, int height, int size, char *data);
	~GameBoard();
	void draw(SDL_Surface *windowSurface);
	int movePlayer(Dir d);
	int move(Dir d, int x, int y);
private:
	GamePiece ***board;
	int playerx, playery;
	int width, height, size;
};

