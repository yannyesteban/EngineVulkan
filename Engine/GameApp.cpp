#include "GameApp.h"
#include <iostream>
#include "db.h"


static void keyCallback ( GLFWwindow* window, int key, int scancode, int action, int mods ) {
	GameApp* app = reinterpret_cast<GameApp*>(glfwGetWindowUserPointer ( window ));

	if (action == GLFW_PRESS) {
		app->onKeyPress ( key );
	}
	/*
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose ( window, GLFW_TRUE );
			break;
		case GLFW_KEY_W:
			// Procesar tecla 'W'
			std::cout << "Tecla W presionada\n";
			break;
		case GLFW_KEY_A:
			// Procesar tecla 'A'
			std::cout << "Tecla A presionada\n";
			break;
			// Agregar más teclas según sea necesario
		}
	}
	*/
}

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

	glfwSetKeyCallback ( window, keyCallback );

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

void GameApp::onKeyPress ( int key ) {
	switch (key) {
	case GLFW_KEY_W:
		// Lógica específica para la tecla 'W'
		std::cout << "Tecla W presionada\n";
		break;
	case GLFW_KEY_A:
		// Lógica específica para la tecla 'A'
		std::cout << "Tecla A presionada\n";
		break;
		// Agregar más teclas según sea necesario
	}
}

void GameApp::onMouseClick ( int button ) {
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		// Lógica específica para el clic izquierdo
		std::cout << "Clic izquierdo\n";
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		// Lógica específica para el clic derecho
		std::cout << "Clic derecho\n";
		break;
		// Agregar más botones si es necesario
	}
}