#include <iostream>

#include "player.h"

void Player::updatePosition(float deltaTime)
{
	if(Game::key_space || Game::key_left_shift)
	{
		Player::velocity[1] = Game::camera.GetUpDownVelocity(Player::flyMultiplier).y * (Game::key_space ? 1 : -1);
	}
	else
	{
		Player::velocity[1] = 0;
	}
	if(Game::key_w || Game::key_s || Game::key_a || Game::key_d)
	{
		glm::vec3 fbVel = Game::camera.GetForwardBackwardVelocity(deltaTime);
		glm::vec3 lrVel = Game::camera.GetLeftRightVelocity(deltaTime);
		float xVel = (
			(Game::key_w ? fbVel.x : (Game::key_s ? -fbVel.x : 0)) +
			(Game::key_d ? lrVel.x : (Game::key_a ? -lrVel.x : 0))
		);
		float zVel = (
			(Game::key_w ? fbVel.z : (Game::key_s ? -fbVel.z : 0)) +
			(Game::key_d ? lrVel.z : (Game::key_a ? -lrVel.z : 0))
		);
		Player::velocity[0] = xVel;
		Player::velocity[2] = zVel;
	}
	else
	{
		Player::velocity[0] = 0;
		Player::velocity[2] = 0;
	}
	if(Player::position[1] < 0)
	{
		Player::position[1] = 0;
		Player::velocity[1] = 0;
	}
	Player::position[0] += Player::velocity[0];
	Player::position[1] += Player::velocity[1];
	Player::position[2] -= Player::velocity[2];
}

void Player::setPosition(float x, float y, float z)
{
	Player::position[0] = x;
	Player::position[1] = y;
	Player::position[2] = z;
}