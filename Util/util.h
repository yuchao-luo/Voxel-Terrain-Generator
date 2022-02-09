#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstring>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "../config.h"

void uploadVertices(
	GLuint *buffers, int bufferCount, 
	const std::vector<std::vector<float>> &vertices, 
	const std::vector<int> &vertexSizes);
void useBuffer(GLuint buffer);
void useTexture(GLuint texture);

#endif
