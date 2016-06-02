#include <stdio.h>
#include "GamePiece.h"

const char *imgWall = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\brick.bmp";
const char *imgSwitch = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\switch.bmp";
const char *imgCrate = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\crate.bmp";
const char *imgPlayer = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\player.bmp";
const char *imgCrateSwitch = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\crateswitch.bmp";
const char *imgPlayerSwitch = "C:\\Users\\xtodd\\Documents\\Visual Studio 2015\\Projects\\PushyCrate\\Debug\\res\\playerswitch.bmp";

GamePiece::GamePiece(PieceType t)
{
	type = t;
	switch (type) {
	case type_wall:
		image = SDL_LoadBMP(imgWall);
		break;
	case type_switch:
		image = SDL_LoadBMP(imgSwitch);
		break;
	case type_crate:
		image = SDL_LoadBMP(imgCrate);
		break;
	case type_player:
		image = SDL_LoadBMP(imgPlayer);
		break;
	case type_crate_switch:
		image = SDL_LoadBMP(imgCrateSwitch);
		break;
	case type_player_switch:
		image = SDL_LoadBMP(imgPlayerSwitch);
		break;
	}
	if (image == NULL) {
		fprintf(stderr, "Could not load image: %s", SDL_GetError());
	}
}

GamePiece::~GamePiece()
{
	SDL_FreeSurface(image);
}

void GamePiece::draw(SDL_Surface *windowSurface, int x, int y, int size)
{
	SDL_Rect r {x*size, y*size, size, size};
	SDL_BlitScaled(image, NULL, windowSurface, &r);
}

void GamePiece::pressSwitch()
{
	switch (type) {
	case type_crate:
		SDL_FreeSurface(image);
		image = SDL_LoadBMP(imgCrateSwitch);
		type = type_crate_switch;
		break;
	case type_player:
		SDL_FreeSurface(image);
		image = SDL_LoadBMP(imgPlayerSwitch);
		type = type_player_switch;
		break;
	default:
		printf("Objects other than player and crate cannot press switches!\n");
		break;
	}
}

void GamePiece::unpressSwitch()
{
	switch (type) {
	case type_crate_switch:
		SDL_FreeSurface(image);
		image = SDL_LoadBMP(imgCrate);
		type = type_crate;
		break;
	case type_player_switch:
		SDL_FreeSurface(image);
		image = SDL_LoadBMP(imgPlayer);
		type = type_player;
		break;
	default:
		printf("Object wasn't on switch!\n");
		break;
	}
}

PieceType GamePiece::getType()
{
	return type;
}
