#pragma once
#include "GamePiece.h"
#include "PrevMove.h"

enum Dir { dir_right, dir_up, dir_left, dir_down };

class GameBoard
{
public:
	GameBoard(int width, int height, int size, char *data);
	~GameBoard();
	void draw(SDL_Surface *windowSurface);
	int movePlayer(Dir d);
	PrevMove *move(Dir d, int x, int y);
	void reset();
	void undo();
	void redo();
	void solve();
	int getNumSwitches();
	int getNumSwitchesPressed();
private:
	GamePiece ***board;
	int playerx, playery;
	int width, height, size;
	char *resetData;
	PrevMove *prev;
	PrevMove *rprev;
	int numSwitches, numSwitchesPressed;
};

