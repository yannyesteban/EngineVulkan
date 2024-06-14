#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <array>
#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace SEVIAN {
    // Definición de un vértice
    struct Vertex
    {
        glm::vec3 position; // Posición del vértice
        glm::vec3 normal;   // Normal del vértice
        glm::vec2 texCoord; // Coordenada de textura
        glm::vec3 color;    // Color del vértice
        bool operator==( const Vertex& other ) const {
            return position == other.position &&
                normal == other.normal &&
                texCoord == other.texCoord &&
                color == other.color;
        }
    };

    // Definición de un material
    struct Material
    {
        glm::vec4 ambient;   // Componente ambiental
        glm::vec4 diffuse;   // Componente difusa
        glm::vec4 specular;  // Componente especular
        float shininess;     // Brillo
    };

    // Definición de una textura
    struct Texture
    {
        std::string type;  // Tipo de textura (e.g., "diffuse", "specular")
        std::string path;  // Ruta del archivo de textura
    };

    // Definición de una malla
    struct Mesh
    {
        std::vector<Vertex> vertices;   // Lista de vértices
        std::vector<uint32_t> indices;  // Lista de índices
        std::vector<Texture> textures;  // Lista de texturas
        Material material;              // Material de la malla
    };

    // Definición de una entidad 3D
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

        static VkVertexInputBindingDescription getBindingDescription () {
            VkVertexInputBindingDescription bindingDescription {};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof ( Vertex );
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions () {
            std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions {};

            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof ( Vertex, position );

            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof ( Vertex, normal );

            attributeDescriptions[2].binding = 0;
            attributeDescriptions[2].location = 2;
            attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
            attributeDescriptions[2].offset = offsetof ( Vertex, texCoord );

            attributeDescriptions[3].binding = 0;
            attributeDescriptions[3].location = 3;
            attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[3].offset = offsetof ( Vertex, color );

            return attributeDescriptions;
        }
    };

    Entity3D createCubeEntity ();
    Entity3D createSquareEntity ();
}
