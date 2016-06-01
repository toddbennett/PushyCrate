#pragma once
#include "SDL.h"

enum PieceType {type_wall, type_switch, type_crate, type_player, type_crate_switch, type_player_switch};

class GamePiece
{
public:
	GamePiece(PieceType t);
	~GamePiece();
	void draw(SDL_Surface *windowSurface, int x, int y);
	void pressSwitch();
	void unpressSwitch();
	PieceType getType();
private:
	PieceType type;
	SDL_Surface *image;
};

