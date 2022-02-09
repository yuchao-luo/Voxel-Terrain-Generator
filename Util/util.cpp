#include "util.h"

void uploadVertices(
	GLuint *buffers, int bufferCount, 
	const std::vector<std::vector<float>> &vertices, 
	const std::vector<int> &vertexSizes)
{
	for(int i = 0; i < bufferCount; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[i]);
		glBufferData(
			GL_ARRAY_BUFFER, 
			vertexSizes[i] * sizeof(float), 
			&(vertices[i][0]), 
			GL_STATIC_DRAW
		);
	}
}

void useBuffer(GLuint buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// position attribute
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE, 
		8 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(
		1, 2, 
		GL_FLOAT, GL_FALSE, 
		8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(
		2, 3, 
		GL_FLOAT, GL_FALSE, 
		8 * sizeof(float), (void *)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void useTexture(GLuint texture)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
}
