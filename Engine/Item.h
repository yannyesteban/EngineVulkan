#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Data3D{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;


	bool operator==(const Data3D& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};
class Item
{
public:
	const char* id;
	std::vector<Data3D> data;
	std::vector<uint32_t> indices;
	const char* texture;

	std::vector<const char*> shaders;
};

