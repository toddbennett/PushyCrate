#include <stdio.h>
#include "GameBoard.h"

GameBoard::GameBoard(int w, int h, int s, char *data)
{
	playerx = -1;
	playery = -1;
	width = w;
	height = h;
	size = s;

	board = new GamePiece**[width];
	for (int i = 0; i < width; i++) {
		board[i] = new GamePiece*[height];
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			switch (data[j*width + i]) {
			case '0':
				board[i][j] = NULL;
				break;
			case 'W':
				board[i][j] = new GamePiece(type_wall);
				break;
			case 'S':
				board[i][j] = new GamePiece(type_switch);
				break;
			case 'P':
				board[i][j] = new GamePiece(type_player);
				if (playerx != -1 || playery != -1) {
					printf("Only one player permitted!!!\n");
					return;
				}
				playerx = i;
				playery = j;
				break;
			case 'C':
				board[i][j] = new GamePiece(type_crate);
				break;
			case 'p':
				board[i][j] = new GamePiece(type_player_switch);
				if (playerx != -1 || playery != -1) {
					printf("Only one player permitted!!!\n");
					return;
				}
				playerx = i;
				playery = j;
				break;
			case 'c':
				board[i][j] = new GamePiece(type_crate_switch);
				break;
			default:
				printf("Unknown character! %c\n", data[j*width + i]);
				return;
			}
		}
	}
	if (playerx == -1 || playery == -1) {
		printf("No player found!\n");
		return;
	}
}


GameBoard::~GameBoard()
{
}

int GameBoard::movePlayer(Dir d) {
	if (move(d, playerx, playery)) {
		int nx = playerx;
		int ny = playery;
		switch (d) {
		case dir_right:
			nx++;
			break;
		case dir_up:
			ny--;
			break;
		case dir_left:
			nx--;
			break;
		case dir_down:
			ny++;
			break;
		}
		playerx = nx;
		playery = ny;
		return 1;
	}
	return 0;
}

int GameBoard::move(Dir d, int x, int y) {
	int nx = x;
	int ny = y;
	switch (d) {
	case dir_right:
		nx++;
		break;
	case dir_up:
		ny--;
		break;
	case dir_left:
		nx--;
		break;
	case dir_down:
		ny++;
		break;
	}

	if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
		return 0;
	}

	GamePiece *p = board[x][y];
	GamePiece *op = board[nx][ny];

	if (p == NULL) {
		printf("Cannot move nothing!\n");
		return 0;
	}

	PieceType ptype = p->getType();

	if (op == NULL) {
		if (ptype == type_crate_switch || ptype == type_player_switch) {
			p->unpressSwitch();
			board[nx][ny] = p;
			board[x][y] = new GamePiece(type_switch);
			return 1;
		}
		else {
			board[nx][ny] = p;
			board[x][y] = NULL;
			return 1;
		}
	}

	PieceType optype = op->getType();
	switch (optype) {
	case type_wall:
		return 0;
	case type_player:
	case type_player_switch:
		printf("Only one player supported!\n");
		return 0;
	case type_switch:
		if (ptype == type_crate_switch || ptype == type_player_switch) {
			board[nx][ny] = p;
			board[x][y] = new GamePiece(type_switch);
			delete op;
			return 1;
		}
		else {
			p->pressSwitch();
			board[nx][ny] = p;
			board[x][y] = NULL;
			delete op;
			return 1;
		}
	case type_crate:
		if (ptype == type_player) {
			if (move(d, nx, ny)) {
				board[nx][ny] = p;
				board[x][y] = NULL;
				return 1;
			}
			return 0;
		}
		if (ptype == type_player_switch) {
			if (move(d, nx, ny)) {
				p->unpressSwitch();
				board[nx][ny] = p;
				board[x][y] = new GamePiece(type_switch);
				return 1;
			}
			return 0;
		}
		return 0;
	case type_crate_switch:
		if (ptype == type_player) {
			if (move(d, nx, ny)) {
				p->pressSwitch();
				op = board[nx][ny];
				delete op;
				board[nx][ny] = p;
				board[x][y] = NULL;
				return 1;
			}
			return 0;
		}
		if (ptype == type_player_switch) {
			if (move(d, nx, ny)) {
				op = board[nx][ny];
				delete op;
				board[nx][ny] = p;
				board[x][y] = new GamePiece(type_switch);
				return 1;
			}
			return 0;
		}
		return 0;
	}
}

void GameBoard::draw(SDL_Surface * windowSurface)
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (board[i][j] != NULL) {
				board[i][j]->draw(windowSurface, i, j, size);
			}
		}
	}
}
