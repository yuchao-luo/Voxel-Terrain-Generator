#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <filesystem>
#include <windows.h>

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SimplexNoise/SimplexNoise.h"

#include "game.h"
#include "Player/player.h"
#include "Window/window_callbacks.h"
#include "UI/fps_view.h"
#include "UI/debug_view.h"
#include "UI/crosshair.h"
#include "Texture/texture.h"
#include "Shader/shader.h"
#include "Cube/cube.h"
#include "Chunk/chunk.h"
#include "Util/util.h"

GLFWwindow* window;

extern "C" {
	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

int setupGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Random Terrain Generator", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		const char* description;
		int code = glfwGetError(&description);
		if(description)
			std::cout << "GLFW Error: " << code << ", description: " << description << std::endl;
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "GLAD: Failed to initialize OpenGL context" << std::endl;
		glfwTerminate();
		return -1;
	}

	// vsync
	// 1 = vsync on
	// 0 = vsync off
	glfwSwapInterval(1);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return 0;
}

int main(int argc, char const *argv[])
{
	int init = setupGLFW();
	if(init != 0)
		return init;

	SimplexNoise simp;
	simp.init();

	Shader worldShader(
		getShaderPath("world_vertex.glsl"), 
		getShaderPath("world_fragment.glsl")
	);
	Shader textShader(
		getShaderPath("text_vertex.glsl"), 
		getShaderPath("text_fragment.glsl")
	);
	Shader crosshairShader(
		getShaderPath("crosshair_vertex.glsl"), 
		getShaderPath("crosshair_fragment.glsl")
	);

	// holds vertices of all the meshes to draw
	std::vector<std::vector<float>> chunkVertices;
	std::vector<int> chunkVertexCounts;
	unsigned int chunkVertexCountTotal = 0;

	constexpr int CHUNK_WIDTH = 1;
	constexpr int CHUNK_LENGTH = 1;
	constexpr int CHUNK_COUNT = CHUNK_WIDTH * CHUNK_LENGTH;

	std::vector<Chunk> chunks;

	for(int i = 0; i < CHUNK_WIDTH; i++)
	{
		for(int j = 0; j < CHUNK_LENGTH; j++)
		{
			int x = i * 16 - ((CHUNK_WIDTH - 1) * 8);
			int z = j * 16 - ((CHUNK_LENGTH - 1) * 8);
			chunks.push_back(Chunk(x, z));
		}
	}

	for(int l = 0; l < CHUNK_COUNT; l++)
	{
		for(int i = 0; i < 16; i++)
		{
			for(int j = 0; j < 16; j++)
			{
				int x = chunks[l].x + i;
				int z = -chunks[l].z + j;

				double n = simp.noise(x * (1.0/64.0), z * (1.0/64.0));
				double n2 = simp.noise(x * (1.0/32.0), z * (1.0/32.0));
				double n3 = simp.noise(x * (1.0/16.0), z * (1.0/16.0));
				int k = 12 + (int)(8 * n + n2 + n3);
				if(k < 0)
					k = 0;
				chunks[l].setBlock(i, k, j, 1);
				int m = k - 1,
					o = k - 3;
				for(; m > o && m > 0; m--)
				{
					chunks[l].setBlock(i, m, j, 2);
				}
				for(; m >= 0; m--)
				{
					chunks[l].setBlock(i, m, j, 3);
				}
			}
		}
	}

	for(int i = 0; i < CHUNK_COUNT; i++)
	{
		chunks[i].generateMesh(&chunks[0], CHUNK_COUNT);
	}
	for(int i = 0; i < CHUNK_COUNT; i++)
	{
			int vertexCount = (int)chunks[i].verts.size();
			chunkVertices.push_back(chunks[i].verts);
			chunkVertexCounts.push_back(vertexCount);
			chunkVertexCountTotal += vertexCount;
	}

	GLuint VAO;
	GLuint chunkVBO[CHUNK_COUNT];

	glGenVertexArrays(1, &VAO);

	glGenBuffers(CHUNK_COUNT, chunkVBO);

	glBindVertexArray(VAO);
	uploadVertices(chunkVBO, CHUNK_COUNT, chunkVertices, chunkVertexCounts);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint world_texture;

	loadTexture(world_texture, "TextureAtlas.png", true, true);
	
	worldShader.use();
	worldShader.setInt("texture1", 0);

	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 model = glm::mat4(1.0f);
	worldShader.use();
	worldShader.setMat4("projection", projection);
	worldShader.setMat4("model", model);

	Game::camera.Position = glm::vec3(Player::position[0], Player::position[1] + 2.4f, Player::position[2]);

	float lastTime = glfwGetTime();
	int nbFrames = 0;
	bool skipDrawing = false;

	std::cout << std::setprecision(3) << std::fixed;
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		nbFrames++;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		
		Player::updatePosition(deltaTime);

		float xpos = Player::position[0],
			  ypos = Player::position[1],
			  zpos = Player::position[2];
		float yaw = Game::camera.Yaw;
		while(yaw < 0)
			yaw += 360;
		while(yaw > 360)
			yaw -= 360;
		if((yaw < 45.0f && yaw >= 0) || (yaw > 315.0f && yaw <= 360.0f))
			Player::facing = Player::East;
		if((yaw < 225.0f && yaw >= 180.0f) || (yaw > 135.0f && yaw <= 180.0f))
			Player::facing = Player::West;
		if((yaw < 135.0f && yaw >= 90.0f) || (yaw > 45.0f && yaw <= 90.0f))
			Player::facing = Player::South;
		if((yaw < 315.0f && yaw >= 270.0f) || (yaw > 225.0f && yaw <= 270.0f))
			Player::facing = Player::North;
		Game::camera.Position = glm::vec3(xpos, ypos + 2.4f, -zpos);

		glClearColor(0.05f, 0.4f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		useTexture(world_texture);

		worldShader.use();

		worldShader.setBool("wireframe", Game::wireframe_mode);
		glPolygonMode(GL_FRONT_AND_BACK, Game::wireframe_mode ? GL_LINE : GL_FILL);

		glm::mat4 view = Game::camera.GetViewMatrix();
		worldShader.setMat4("view", view);

		if(!skipDrawing)
		{
			glBindVertexArray(VAO);
			for(int i = 0; i < CHUNK_COUNT; i++)
			{
				useBuffer(chunkVBO[i]);
				glDrawArrays(GL_TRIANGLES, 0, chunkVertexCounts[i] / 8);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(CHUNK_COUNT, chunkVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();

	return 0;
}
