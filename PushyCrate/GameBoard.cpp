#include <stdio.h>
#include "GameBoard.h"

GameBoard::GameBoard(int w, int h, int s, char *data)
{
	playerx = -1;
	playery = -1;
	width = w;
	height = h;
	size = s;
	resetData = data;
	prev = NULL;

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

void GameBoard::undo() {
	if (prev == NULL) {
		return;
	}
	int nx = playerx;
	int ny = playery;
	switch (prev->direction) {
	case 'r':
		nx--;
		break;
	case 'u':
		ny++;
		break;
	case 'l':
		nx++;
		break;
	case 'd':
		ny--;
		break;
	}
	board[nx][ny] = board[playerx][playery];
	board[playerx][playery] = NULL;

	GamePiece *player = board[nx][ny];
	if (prev->player_on) {
		player->unpressSwitch();
		board[playerx][playery] = new GamePiece(type_switch);
	}
	if (prev->player_off) {
		player->pressSwitch();
	}

	if (prev->crate) {
		int cratex = playerx;
		int cratey = playery;
		int cratenx = playerx;
		int crateny = playery;
		switch (prev->direction) {
		case 'r':
			cratex++;
			break;
		case 'u':
			cratey--;
			break;
		case 'l':
			cratex--;
			break;
		case 'd':
			cratey++;
			break;
		}
		board[cratenx][crateny] = board[cratex][cratey];
		board[cratex][cratey] = NULL;
		GamePiece *crate = board[cratenx][crateny];
		if (prev->crate_on) {
			crate->unpressSwitch();
			board[cratex][cratey] = new GamePiece(type_switch);
		}
		if (prev->crate_off) {
			crate->pressSwitch();
		}
	}

	playerx = nx;
	playery = ny;

	PrevMove *temp = prev;
	prev = prev->prev;
	delete temp;
}

void GameBoard::reset() {
	playerx = -1;
	playery = -1;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (board[i][j] != NULL) {
				delete board[i][j];
			}
			switch (resetData[j*width + i]) {
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
				printf("Unknown character! %c\n", resetData[j*width + i]);
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
	delete[] resetData;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (board[i][j] != NULL) {
				delete board[i][j];
			}
		}
	}
	delete[] board;
}

int GameBoard::movePlayer(Dir d) {
	PrevMove *t = move(d, playerx, playery);
	if (t) {
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
		t->prev = prev;
		prev = t;
		return 1;
	}
	return 0;
}

PrevMove *GameBoard::move(Dir d, int x, int y) {
	PrevMove *prev = new PrevMove();
	int nx = x;
	int ny = y;
	switch (d) {
	case dir_right:
		prev->direction = 'r';
		nx++;
		break;
	case dir_up:
		prev->direction = 'u';
		ny--;
		break;
	case dir_left:
		prev->direction = 'l';
		nx--;
		break;
	case dir_down:
		prev->direction = 'd';
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
			prev->player_off = true;
			return prev;
		}
		else {
			board[nx][ny] = p;
			board[x][y] = NULL;
			return prev;
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
		prev->player_on = true;
		if (ptype == type_crate_switch || ptype == type_player_switch) {
			board[nx][ny] = p;
			board[x][y] = new GamePiece(type_switch);
			delete op;
			prev->player_off = true;
			return prev;
		}
		else {
			p->pressSwitch();
			board[nx][ny] = p;
			board[x][y] = NULL;
			delete op;
			return prev;
		}
	case type_crate:
		prev->crate = true;
		if (ptype == type_player) {
			PrevMove *t = move(d, nx, ny);
			if (t) {
				board[nx][ny] = p;
				board[x][y] = NULL;
				if (t->player_on) {
					prev->crate_on = true;
				}
				return prev;
			}
			return 0;
		}
		if (ptype == type_player_switch) {
			prev->player_off = true;
			PrevMove *t = move(d, nx, ny);
			if (t) {
				p->unpressSwitch();
				board[nx][ny] = p;
				board[x][y] = new GamePiece(type_switch);
				if (t->player_on) {
					prev->crate_on = true;
				}
				return prev;
			}
			return 0;
		}
		return 0;
	case type_crate_switch:
		prev->crate = true;
		prev->crate_off = true;
		if (ptype == type_player) {
			PrevMove *t = move(d, nx, ny);
			if (t) {
				p->pressSwitch();
				op = board[nx][ny];
				delete op;
				board[nx][ny] = p;
				board[x][y] = NULL;
				if (t->player_on) {
					prev->crate_on = true;
				}
				return prev;
			}
			return 0;
		}
		if (ptype == type_player_switch) {
			prev->player_off = true;
			PrevMove *t = move(d, nx, ny);
			if (t) {
				op = board[nx][ny];
				delete op;
				board[nx][ny] = p;
				board[x][y] = new GamePiece(type_switch);
				if (t->player_on) {
					prev->crate_on = true;
				}
				return prev;
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
