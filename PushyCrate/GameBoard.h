#pragma once
#include "GamePiece.h"

enum Dir { dir_right, dir_up, dir_left, dir_down };

class GameBoard
{
public:
	GameBoard();
	~GameBoard();
	void draw(SDL_Surface *windowSurface);
	int movePlayer(Dir d);
	int move(Dir d, int x, int y);
private:
	GamePiece *board[10][10];
	int playerx, playery;
};

