#include <stdio.h>
#include "GameBoard.h"

GameBoard::GameBoard()
{
	playerx = 1;
	playery = 1;

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			board[i][j] = NULL;
		}
	}

	board[0][0] = new GamePiece(type_wall);
	board[0][1] = new GamePiece(type_wall);
	board[0][2] = new GamePiece(type_wall);
	board[0][3] = new GamePiece(type_wall);
	board[0][4] = new GamePiece(type_wall);
	board[0][5] = new GamePiece(type_wall);

	board[1][0] = new GamePiece(type_wall);
	board[1][1] = new GamePiece(type_player);
	board[1][2] = NULL;
	board[1][3] = new GamePiece(type_crate);
	board[1][4] = new GamePiece(type_switch);
	board[1][5] = new GamePiece(type_wall);

	board[2][0] = new GamePiece(type_wall);
	board[2][1] = new GamePiece(type_wall);
	board[2][2] = new GamePiece(type_wall);
	board[2][3] = new GamePiece(type_wall);
	board[2][4] = new GamePiece(type_wall);
	board[2][5] = new GamePiece(type_wall);
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
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (board[i][j] != NULL) {
				board[i][j]->draw(windowSurface, i, j);
			}
		}
	}
}
