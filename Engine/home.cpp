#include "GameApp.h"
#include "Entity.h"
#include <fstream>

static void x2000 ( GLFWwindow* window, int width, int height ) {
	auto app = reinterpret_cast<GameApp*>(glfwGetWindowUserPointer ( window ));
	app->engine->framebufferResized = true;
	app->test ( width, height );
}

int main () {


	SEVIAN::Entity3D cubeEntity = SEVIAN::createCubeEntity ();


	VulkanEngine engine;
	std::vector<Texture> textures {
		{"main","textures/fauna-marina.jpg","" },
		{ "room", "textures/viking_room.png","" },
		{ "fauna","textures/texture.jpg","" },
		{ "bandera","textures/Bandera-Suiza.jpg","" }
	};
	std::vector<Shader> shaders {

		{"vert", "shaders/vert.spv", "vert", "main", {1,1,3}},
		{"frag", "shaders/frag.spv", "frag", "main", {1,1,3}}
	};


	float z = 0.6;
	float y = 0.3;

	glm::mat4 model = glm::mat4 ( 1.0f );

	float translateX = 1.0f;  // Ajusta según sea necesario
	float translateY = 0.0f;  // Ajusta según sea necesario
	float translateZ = 0.0f;  // Ajusta según sea necesario



	std::vector<Data3D> data;
	std::vector<uint32_t> indices;
	loadModel ( "models/viking_room.obj", data, indices );



	Item ii = {
		"xroom",
		data, indices, "room"

	};



	std::vector<Item> items { ii,
		{ "item1",
		{
			{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		},
		{0, 1, 2, 2, 3, 0} , "room", {"vert", "frag"}},
		{"item1", {
			{{-0.5f, -0.5f, 0.0f + z}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f + z}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f + z}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f + z}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		},
		{0, 1, 2, 2, 3, 0} , "bandera", {"vert", "frag"}},
		{"item1", {
			{{-0.5f, -0.5f, 0.0f + y}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
			{{0.5f, -0.5f, 0.0f + y}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
			{{0.5f, 0.5f, 0.0f + y}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
			{{-0.5f, 0.5f, 0.0f + y}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
		},
		{0, 1, 2, 2, 3, 0} , "fauna", {"vert", "frag"}}
	};


	GameApp app;
	app.title = "yanny esteban";
	app.resizeCallback = x2000;
	app.textures = textures;
	app.shaders = shaders;
	app.items = items;

	engine.textures = textures;
	engine.shaders = shaders;
	engine.items = items;

	app.engine = &engine;

	try {

		app.initWindow ();
		app.run ();
		app.doLoop ();
		engine.end ();
		engine.cleanup ();
	}
	catch (const std::exception& e) {
		std::cerr << e.what () << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}