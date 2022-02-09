#include "window_callbacks.h"
#include "../game.h"

namespace Mouse
{
	float lastX = SCR_WIDTH / 2.0f;
	float lastY = SCR_HEIGHT / 2.0f;
	bool firstMouse = true;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if(action == GLFW_PRESS)
			Game::mouse_left_button = true;
		else if(action == GLFW_RELEASE)
			Game::mouse_left_button = false;
	}
	if(button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if(action == GLFW_PRESS)
			Game::mouse_right_button = true;
		else if(action == GLFW_RELEASE)
			Game::mouse_right_button = false;
	}
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	// Wireframe mode
	if(key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		Game::wireframe_mode ^= true;
	}
	// Movement
	if(key == GLFW_KEY_W)
	{
		if(action == GLFW_PRESS)
			Game::key_w = true;
		else if(action == GLFW_RELEASE)
			Game::key_w = false;
	}
	if(key == GLFW_KEY_S)
	{
		if(action == GLFW_PRESS)
			Game::key_s = true;
		else if(action == GLFW_RELEASE)
			Game::key_s = false;
	}
	if(key == GLFW_KEY_A)
	{
		if(action == GLFW_PRESS)
			Game::key_a = true;
		else if(action == GLFW_RELEASE)
			Game::key_a = false;
	}
	if(key == GLFW_KEY_D)
	{
		if(action == GLFW_PRESS)
			Game::key_d = true;
		else if(action == GLFW_RELEASE)
			Game::key_d = false;
	}
	if(key == GLFW_KEY_SPACE)
	{
		if(action == GLFW_PRESS)
			Game::key_space = true;
		else if(action == GLFW_RELEASE)
			Game::key_space = false;
	}
	if(key == GLFW_KEY_LEFT_SHIFT)
	{
		if(action == GLFW_PRESS)
			Game::key_left_shift = true;
		else if(action == GLFW_RELEASE)
			Game::key_left_shift = false;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	using namespace Mouse;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
	lastX = xpos;
	lastY = ypos;
	Game::camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	// std::cout << "Viewport " << width << ", " << height << std::endl;
}