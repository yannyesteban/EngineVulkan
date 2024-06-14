#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include "tiny_obj_loader.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <map>
#include <algorithm>
#include <unordered_map>



struct ShaderProgram
{
	VkShaderModule vertexShader;
	VkShaderModule fragmentShader;
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct Material
{
	ShaderProgram* shaderProgram;  // Programa de shader asociado
	glm::vec4 ambient;             // Componente ambiental
	glm::vec4 diffuse;             // Componente difusa
	glm::vec4 specular;            // Componente especular
	float shininess;               // Brillo
};

struct VertexX
{
	glm::vec3 position; // Posición del vértice
	glm::vec3 normal;   // Normal del vértice
	glm::vec2 texCoord; // Coordenada de textura
};

struct Mesh
{
	std::vector<VertexX> vertices;   // Lista de vértices
	std::vector<uint32_t> indices;  // Lista de índices
	Material* material;             // Material de la malla
};

struct Entity3D
{

	std::string name;          // Nombre de la entidad
	std::vector<Mesh> meshes;  // Lista de mallas que componen la entidad
	glm::vec3 position;        // Posición de la entidad en el espacio
	glm::vec3 rotation;        // Rotación de la entidad (en grados)
	glm::vec3 scale;           // Escala de la entidad
	glm::mat4 modelMatrix;     // Matriz de modelo de la entidad

	// Función para actualizar la matriz de modelo en base a la posición, rotación y escala
	void updateModelMatrix () {
		modelMatrix = glm::mat4 ( 1.0f );
		modelMatrix = glm::translate ( modelMatrix, position );
		modelMatrix = glm::rotate ( modelMatrix, glm::radians ( rotation.x ), glm::vec3 ( 1.0f, 0.0f, 0.0f ) );
		modelMatrix = glm::rotate ( modelMatrix, glm::radians ( rotation.y ), glm::vec3 ( 0.0f, 1.0f, 0.0f ) );
		modelMatrix = glm::rotate ( modelMatrix, glm::radians ( rotation.z ), glm::vec3 ( 0.0f, 0.0f, 1.0f ) );
		modelMatrix = glm::scale ( modelMatrix, scale );
	}
};

struct Data3D{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec2 texCoord;


	bool operator==(const Data3D& other) const {
		return pos == other.pos && color == other.color && texCoord == other.texCoord;
	}
};
void loadModel ( const char* path, std::vector<Data3D>& vertices, std::vector<uint32_t>& indices );
namespace std {
	template<> struct hash<Data3D>
	{
		size_t operator()( Data3D const& vertex ) const {
			return ((hash<glm::vec3> ()(vertex.pos) ^ (hash<glm::vec3> ()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2> ()(vertex.texCoord) << 1);
		}
	};
}
class Item
{
public:
	const char* id;
	std::vector<Data3D> data;
	std::vector<uint32_t> indices;
	const char* texture;

	std::vector<const char*> shaders;
};

