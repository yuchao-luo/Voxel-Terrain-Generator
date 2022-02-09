#ifndef CHUNKS_H
#define CHUNKS_H

#include <vector>

#include "../Cube/cube.h"

typedef unsigned short int BlockType;

namespace Block
{
	enum BlockType
	{
		Air,
		Grass,
		Dirt,
		Stone
	};
	inline float textureCoord[][3][2] = {
		// side			bottom			top
		{{16.0f, 0.0f}, {32.0f, 0.0f}, {0.0f, 0.0f}},	// grass
		{{32.0f, 0.0f}, {32.0f, 0.0f}, {32.0f, 0.0f}},	// dirt
		{{48.0f, 0.0f}, {48.0f, 0.0f}, {48.0f, 0.0f}}	// stone
	};
}

class Chunk
{
public:
	int x;
	int z;
	int maxY;
	// int faces;
	std::vector<float> verts;
	std::vector<Cube::Face> frontFaces;
	std::vector<Cube::Face> backFaces;
	std::vector<Cube::Face> leftFaces;
	std::vector<Cube::Face> rightFaces;
	std::vector<Cube::Face> bottomFaces;
	std::vector<Cube::Face> topFaces;
	BlockType blocks[256][16][16];

	Chunk();
	Chunk(int, int);
	BlockType getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, BlockType blockType);
	void updateVertices();
	static int getChunkFromXZ(int x, int z, Chunk *chunks, int count);
	float getMaxYFromXZ(int x, int z);
	void generateMesh(Chunk *chunks, int count);
private:
};

#endif
