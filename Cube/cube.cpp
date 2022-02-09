#include <iostream>
#include "cube.h"

Cube::Vertex::Vertex()
{
	this->position[0] = 0;
	this->position[1] = 0;
	this->position[2] = 0;
	this->texture[0] = 0;
	this->texture[1] = 0;
	this->normal[0] = 0;
	this->normal[1] = 0;
	this->normal[2] = 0;
}

Cube::Vertex::Vertex(
	float px, float py, float pz, 
	float u, float v, 
	float nx, float ny, float nz)
{
	this->position[0] = px;
	this->position[1] = py;
	this->position[2] = pz;
	this->texture[0] = u;
	this->texture[1] = v;
	this->normal[0] = nx;
	this->normal[1] = ny;
	this->normal[2] = nz;
}

Cube::Face makeFace(
	float size, 
	float textureWidth, float textureHeight, 
	float textureCoord[], float textureSize, 
	float x, float y, float z, int w)
{
	Cube::Face face = {};
	for(int i = 0; i < 6; i++)
	{
		int n = Cube::indices[i];
		face.vertices[i] = Cube::Vertex(
			x + size * Cube::positions[n][w][0],
			y + size * Cube::positions[n][w][1],
			z + size * Cube::positions[n][w][2],
			(textureCoord[0] + textureSize * Cube::uvs[n][0]) / textureWidth,
			(textureCoord[1] + textureSize * Cube::uvs[n][1]) / textureHeight,
			Cube::normals[w][0],
			Cube::normals[w][1],
			Cube::normals[w][2]
		);
		// face.vertices[i].position[0] = x + size * Cube::positions[n][w][0];
		// face.vertices[i].position[1] = y + size * Cube::positions[n][w][1];
		// face.vertices[i].position[2] = z + size * Cube::positions[n][w][2];
		// face.vertices[i].texture[0] = (textureCoord[0] + textureSize * Cube::uvs[n][0]) / textureWidth;
		// face.vertices[i].texture[1] = (textureCoord[1] + textureSize * Cube::uvs[n][1]) / textureHeight;
		// face.vertices[i].normal[0] = Cube::normals[w][0];
		// face.vertices[i].normal[1] = Cube::normals[w][1];
		// face.vertices[i].normal[2] = Cube::normals[w][2];
	}
	return face;
}

std::vector<Cube::Face> makeCube(
	float size, 
	float textureWidth, float textureHeight, 
	float textureCoord[][2], float textureSize,
	float x, float y, float z)
{
	std::vector<Cube::Face> faces;
	for(int i = 0; i < 6; i++)
	{
		faces.push_back(makeFace(size, textureWidth, textureHeight, textureCoord[i], textureSize, x, y, z, i));
	}
	return faces;
}
