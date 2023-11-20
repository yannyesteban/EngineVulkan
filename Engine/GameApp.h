#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include "Item.h"
#include "Actor.h"
#include "VulkanEngine.h"
class GameApp
{
private: 
	GLFWwindow* window;
	uint32_t width = 800;
	uint32_t height = 600;

	
	
public:
	~GameApp();
	VulkanEngine * engine;
	const char* title;

	std::vector<Texture> textures;
	std::vector<Shader> shaders;
	std::vector<Item> items;
	std::vector<Actor> actors;

	void (*resizeCallback)(GLFWwindow* window, int width, int height);
	void initWindow();
	void run();
	void doLoop();
	void doFrame();
	void test(uint32_t width, uint32_t height);
};

