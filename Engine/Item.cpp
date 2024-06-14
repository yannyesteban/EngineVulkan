#include "Item.h"


void loadModel ( const char* path, std::vector<Data3D>& vertices, std::vector<uint32_t>& indices ) {



	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj ( &attrib, &shapes, &materials, &warn, &err, path )) {
		throw std::runtime_error ( warn + err );
	}

	std::unordered_map<Data3D, uint32_t> uniqueVertices {};

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Data3D vertex {};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]
			};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.color = { 1.0f, 1.0f, 1.0f };

			if (uniqueVertices.count ( vertex ) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size ());
				vertices.push_back ( vertex );
			}

			indices.push_back ( uniqueVertices[vertex] );

		}
	}

}