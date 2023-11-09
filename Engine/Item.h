#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Data3D{
	glm::vec3 posi;
	glm::vec3 color;
	glm::vec3 texCoord;
};
class Item
{
public:
	const char* id;
	std::vector<Data3D> data;
	std::vector<uint16_t> indices;
	const char* texture;

	std::vector<const char*> shaders;
};

