#ifndef GAME_H
#define GAME_H

#include "Camera/camera.h"

namespace Game
{
	inline Camera camera(glm::vec3(0, 0, 0));
	inline bool wireframe_mode = false;
	inline bool key_w = false;
	inline bool key_a = false;
	inline bool key_s = false;
	inline bool key_d = false;
	inline bool key_space = false;
	inline bool key_left_shift = false;
	inline bool mouse_left_button = false;
	inline bool mouse_right_button = false;
}

#endif