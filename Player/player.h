#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>

#include "../game.h"

namespace Player
{
	enum FacingDirection
	{
		North,
		East,
		South,
		West
	};
	inline float position[3] = {0.0f, 13.0f, 0.0f};
	inline float velocity[3] = {0.0f, 0.0f, 0.0f};
	inline float flyMultiplier = 0.01f;
	inline auto facing = North;

	void updatePosition(float deltaTime);
	void setPosition(float x, float y, float z);
}

#endif