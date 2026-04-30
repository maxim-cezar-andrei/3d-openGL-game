#pragma once
#include <glm/glm.hpp>

enum GameState
{
	STATE_MAIN_MENU,
	STATE_PLAYING,
	STATE_GAME_OVER
};

enum TipObiect
{
	SAFE,
	UNSAFE,
	WALL,
	FLOOR,
	FINISH
};

struct Obiect
{
	glm::vec3 pozitie;
	glm::vec3 scala;
	TipObiect tip;
};
