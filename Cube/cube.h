#ifndef CUBE_H
#define CUBE_H

#include <vector>

namespace Cube
{
	inline const float positions[4][6][3] = {
		// front		back		left			right		bottom			top
		{{-1, -1,  1}, { 1, -1, -1}, {-1, -1, -1}, { 1, -1,  1}, { 1, -1,  1}, { 1,  1, -1}},
		{{ 1, -1,  1}, {-1, -1, -1}, {-1, -1,  1}, { 1, -1, -1}, {-1, -1,  1}, {-1,  1, -1}},
		{{ 1,  1,  1}, {-1,  1, -1}, {-1,  1,  1}, { 1,  1, -1}, {-1, -1, -1}, {-1,  1,  1}},
		{{-1,  1,  1}, { 1,  1, -1}, {-1,  1, -1}, { 1,  1,  1}, { 1, -1, -1}, { 1,  1,  1}},
	};
	inline const float normals[6][3] = {
		// front		back		left
		{ 0,  0,  1}, { 0,  0, -1}, {-1,  0,  0}, 
		// right		bottom		top
		{ 1,  0,  0}, { 0, -1,  0}, { 0,  1,  0}
	};
	inline const float uvs[4][2] = {{0, 0}, {1, 0}, {1, 1}, {0, 1}};
	inline const int indices[6] = {0, 1, 2, 2, 3, 0};
	class Vertex
	{
	public:
		float position[3];
		float texture[2];
		float normal[3];
		Vertex();
		Vertex(float, float, float, float, float, float, float, float);
	};
	class Face
	{
	public:
		Vertex vertices[6];
		bool checkForOverlap(Face face);
	};
}

Cube::Face makeFace(
	float size, 
	float textureWidth, float textureHeight, 
	float textureCoord[], float textureSize, 
	float x, float y, float z, int w);

std::vector<Cube::Face> makeCube(
	float size, 
	float textureWidth, float textureHeight, 
	float textureCoord[][2], float textureSize,
	float x, float y, float z);

#endif