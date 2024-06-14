#include "GameApp.h"
#include <iostream>
#include "db.h"

void GameApp::run()
{
	engine->start();
}

GameApp::~GameApp()
{

	db("GameApp");

	glfwDestroyWindow(window);

	glfwTerminate();
}

void GameApp::initWindow()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, this->resizeCallback);
	engine->window = window;
}

void GameApp::doLoop()
{

	
	db("init all");
	engine->textures = textures;
	engine->shaders = shaders;
	engine->items = items;
	

	int k = 0;
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		doFrame();

		if (k++ >= 4) {
			//break;
		}
	}
	
	

}

void GameApp::doFrame()
{
	engine->draw();
}

void GameApp::test(uint32_t width, uint32_t height)
{
	std::cout << " width: " << width << " height: " << height << "\n";
}
