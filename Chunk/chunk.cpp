#include <iostream>

#include "chunk.h"
#include "../Texture/texture.h"

Chunk::Chunk() : x(0), z(0)
{
	for(int i = 0; i < 16; i++)
		for(int j = 0; j < 16; j++)
			for(int k = 0; k < 256; k++)
				this->blocks[i][j][k] = 0;
	this->maxY = 0;
}

Chunk::Chunk(int x, int z) : x(x), z(z)
{
	for(int i = 0; i < 16; i++)
		for(int j = 0; j < 16; j++)
			for(int k = 0; k < 256; k++)
				this->blocks[i][j][k] = 0;
	this->maxY = 0;
}

BlockType Chunk::getBlock(int x, int y, int z)
{
	return this->blocks[y][z][x];
}

void Chunk::setBlock(int x, int y, int z, BlockType blockType)
{
	if(blockType != 0 && y > this->maxY)
	{
		this->maxY = y;
	}
	this->blocks[y][z][x] = blockType;
}

void Chunk::updateVertices()
{
	for(auto face : this->frontFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	}
	for(auto face : this->backFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	}
	for(auto face : this->leftFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	}
	for(auto face : this->rightFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	}
	for(auto face : this->bottomFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	for(auto face : this->topFaces)
	{
		for(int i = 0; i < 6; i++)
		{
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].position[j]);
			for(int j = 0; j < 2; j++)
				this->verts.push_back(face.vertices[i].texture[j]);
			for(int j = 0; j < 3; j++)
				this->verts.push_back(face.vertices[i].normal[j]);
		}
	}
	}
}

int Chunk::getChunkFromXZ(int x, int z, Chunk *chunks, int count)
{
	int chunkIndex = -1;
	for(int i = 0; i < count; i++)
	{
		Chunk &chunk = chunks[i];
		if(x >= chunk.x && x < chunk.x + 16 && z >= chunk.z && z < chunk.z + 16)
		{
			chunkIndex = i;
			break;
		}
	}
	return chunkIndex;
}

float Chunk::getMaxYFromXZ(int x, int z)
{
	float maxY = this->maxY;
	for(int i = this->maxY; i > 0; i--)
	{
		if(this->getBlock(x, i, z) != 0)
		{
			maxY = i;
			break;
		}
	}
	return maxY;
}

void Chunk::generateMesh(Chunk *chunks, int count)
{
	float textureWidth = Texture::block_texture_width;
	float textureHeight = Texture::block_texture_height;
	for(int x = 0; x < 16; x++)
	{
		for(int z = 0; z < 16; z++)
		{
			for(int y = 0; y <= this->maxY; y++)
			{
				BlockType blockType = this->getBlock(x, y, z);
				if(blockType != Block::Air)
				{
					if(x > 0 && this->getBlock(x - 1, y, z) == Block::Air)
					{
						this->leftFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][0], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 2
							)
						);
					}
					if(x < 15 && this->getBlock(x + 1, y, z) == Block::Air)
					{
						this->rightFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][0], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 3
							)
						);
					}
					if(z > 0 && this->getBlock(x, y, z - 1) == Block::Air)
					{
						this->frontFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][0], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 0
							)
						);
					}
					if(z < 15 && this->getBlock(x, y, z + 1) == Block::Air)
					{
						this->backFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][0], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 1
							)
						);
					}
					if((y < 255 && this->getBlock(x, y + 1, z) == Block::Air) || y == this->maxY)
					{
						this->topFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][2], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 5
							)
						);
					}
					if((y > 0 && this->getBlock(x, y - 1, z) == Block::Air) || y == 0)
					{
						this->bottomFaces.push_back(
							makeFace(
								0.5f, 
								textureWidth, textureHeight, 
								Block::textureCoord[blockType - 1][1], 16.0f, 
								(float)(this->x + x), (float)y, (float)(this->z - z), 4
							)
						);
					}
					if(x == 0)
					{
						int neighborChunk = Chunk::getChunkFromXZ(this->x - 1, this->z + z, chunks, count);
						if(neighborChunk == -1 || chunks[neighborChunk].getBlock(15, y, z) == Block::Air)
						{
							this->leftFaces.push_back(
								makeFace(
									0.5f, 
									textureWidth, textureHeight, 
									Block::textureCoord[blockType - 1][0], 16.0f, 
									(float)(this->x), (float)y, (float)(this->z - z), 2
								)
							);
						}
					}
					else if(x == 15)
					{
						int neighborChunk = Chunk::getChunkFromXZ(this->x + 16, this->z + z, chunks, count);
						if(neighborChunk == -1 || chunks[neighborChunk].getBlock(0, y, z) == Block::Air)
						{
							this->rightFaces.push_back(
								makeFace(
									0.5f,
									textureWidth, textureHeight,
									Block::textureCoord[blockType - 1][0], 16.0f,
									(float)(this->x + 15), (float)y, (float)(this->z - z), 3
								)
							);
						}
					}
					if(z == 0)
					{
						int neighborChunk = Chunk::getChunkFromXZ(this->x + x, this->z + 16, chunks, count);
						if(neighborChunk == -1 || chunks[neighborChunk].getBlock(x, y, 15) == Block::Air)
						{
							this->frontFaces.push_back(
								makeFace(
									0.5f,
									textureWidth, textureHeight,
									Block::textureCoord[blockType - 1][0], 16.0f,
									(float)(this->x + x), (float)y, (float)(this->z), 0
								)
							);
						}
					}
					else if(z == 15)
					{
						int neighborChunk = Chunk::getChunkFromXZ(this->x + x, this->z - 1, chunks, count);
						if(neighborChunk == -1 || chunks[neighborChunk].getBlock(x, y, 0) == Block::Air)
						{
							this->backFaces.push_back(
								makeFace(
									0.5f, 
									textureWidth, textureHeight, 
									Block::textureCoord[blockType - 1][0], 16.0f, 
									(float)(this->x + x), (float)y, (float)(this->z - 15), 1
								)
							);
						}
					}
				}
			}
		}
	}
	this->updateVertices();
}
