#pragma once
struct PrevMove
{
	char direction;
	bool crate;
	bool crate_on;
	bool crate_off;
	bool player_on;
	bool player_off;

	PrevMove *prev;
};