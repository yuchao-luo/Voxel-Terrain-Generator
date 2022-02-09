#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <glad/glad.h>

#include "stb_image.h"
#include "../config.h"

namespace Texture
{
	const float block_texture_width = 64.0f;
	const float block_texture_height = 16.0f;
}

std::string getTexturePath(std::string filename);
void loadTexture(GLuint &texture, std::string file, bool isAlpha, bool repeat);

#endif