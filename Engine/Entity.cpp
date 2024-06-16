#include "Entity.h"

namespace SEVIAN {
    // Crear un cubo
    Entity3D createCubeEntity () {
        // Definir los vértices del cubo con sus normales y coordenadas de textura
        std::vector<Vertex> vertices = {
            // Front face
            {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Red
            {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Green
            {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Blue
            {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}, // Yellow
            // Back face
            {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 1.0f}}, // Cyan
            {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // Magenta
            {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {0.5f, 0.5f, 0.5f}}, // Grey
            {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}, // White
            // Left face
            {{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}}, // Orange
            {{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f, 0.0f}}, // Lime
            {{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.5f}}, // Aqua
            {{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 1.0f}}, // Violet
            // Right face
            {{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.5f}}, // Pink
            {{ 0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 0.5f, 1.0f}}, // Sky Blue
            {{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f, 1.0f}}, // Light Blue
            {{ 0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.5f}}, // Peach
            // Top face
            {{-0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.5f, 1.0f}}, // Purple
            {{ 0.5f,  0.5f, -0.5f}, { 0.0f, 1.0f,  0.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}}, // Pink
            {{ 0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f,  0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Green
            {{-0.5f,  0.5f,  0.5f}, { 0.0f, 1.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}}, // Yellow
            // Bottom face
            {{-0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 1.0f}, {1.0f, 0.5f, 0.0f}}, // Orange
            {{ 0.5f, -0.5f, -0.5f}, { 0.0f, -1.0f,  0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f, 0.0f}}, // Lime
            {{ 0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.5f}}, // Aqua
            {{-0.5f, -0.5f,  0.5f}, { 0.0f, -1.0f,  0.0f}, {0.0f, 1.0f}, {0.5f, 0.0f, 1.0f}}  // Violet
        };

        // Definir los índices del cubo
        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0, // Front face
            4, 5, 6, 6, 7, 4, // Back face
            8, 9, 10, 10, 11, 8, // Left face
            12, 13, 14, 14, 15, 12, // Right face
            16, 17, 18, 18, 19, 16, // Top face
            20, 21, 22, 22, 23, 20 // Bottom face
        };

        // Crear el material del cubo
        Material cubeMaterial = {
            glm::vec4 ( 1.0f, 0.5f, 0.31f, 1.0f ), // Ambient
            glm::vec4 ( 1.0f, 0.5f, 0.31f, 1.0f ), // Diffuse
            glm::vec4 ( 0.5f, 0.5f, 0.5f, 1.0f ),  // Specular
            32.0f                              // Shininess
        };

        // Crear la malla del cubo
        Mesh cubeMesh;
        cubeMesh.vertices = vertices;
        cubeMesh.indices = indices;
        cubeMesh.material = cubeMaterial;

        // Crear la entidad 3D del cubo
        Entity3D cubeEntity;
        cubeEntity.name = "Cube";
        cubeEntity.meshes.push_back ( cubeMesh );
        cubeEntity.position = glm::vec3 ( 0.0f, 0.0f, 0.0f );
        cubeEntity.rotation = glm::vec3 ( 0.0f, 0.0f, 0.0f );
        cubeEntity.scale = glm::vec3 ( 1.0f, 1.0f, 1.0f );
        cubeEntity.updateModelMatrix ();

        return cubeEntity;
    }


    Entity3D createSquareEntity () {



        std::vector<Vertex> vertices = {
            // Positions         // Normals           // TexCoords // Colors
            {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // Bottom-left
            {{0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Bottom-right
            {{0.5f, 0.5f, 0.0f},   {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}, // Top-right
            {{-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}  // Top-left
        };


        std::vector<uint32_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        Texture texture;
        texture.type = "diffuse";
        texture.path = "path_to_your_texture.png"; // Cambia esto a la ruta real de tu imagen PNG

        Material material;
        material.ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
        material.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
        material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
        material.shininess = 32.0f;

        Mesh mesh;
        mesh.vertices = vertices;
        mesh.indices = indices;
        mesh.textures = { texture };
        mesh.material = material;

        Entity3D entity;
        entity.name = "TexturedSquare";
        entity.meshes = { mesh };
        entity.position = { 0.0f, 0.0f, 0.0f };
        entity.rotation = { 0.0f, 0.0f, 0.0f };
        entity.scale = { 1.0f, 1.0f, 1.0f };
        entity.updateModelMatrix ();

        return entity;
    }
}

namespace std {
    template<> struct hash<glm::vec2>
    {
        size_t operator()( const glm::vec2& v ) const {
            return hash<float> ()(v.x) ^ (hash<float> ()(v.y) << 1);
        }
    };

    template<> struct hash<glm::vec3>
    {
        size_t operator()( const glm::vec3& v ) const {
            size_t seed = 0;
            hash<float> hasher;
            seed ^= hasher ( v.x ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher ( v.y ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= hasher ( v.z ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            return seed;
        }
    };
}

namespace std {
    template<> struct hash<SEVIAN::Vertex>
    {
        size_t operator()( SEVIAN::Vertex const& vertex ) const {
            size_t seed = 0;
            hash<glm::vec3> vec3Hasher;
            hash<glm::vec2> vec2Hasher;

            seed ^= vec3Hasher ( vertex.position ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= vec3Hasher ( vertex.normal ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= vec3Hasher ( vertex.color ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            seed ^= vec2Hasher ( vertex.texCoord ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

            return seed;
        }
    };
}