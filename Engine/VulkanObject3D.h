#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"
#include "Device.h"
#include "Entity.h"

class VulkanObject3D
{

public:
	VertexBuffer vertex;
	VertexBuffer indices;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

	std::vector<VkDescriptorSet> descriptorSets;

	size_t indicesSizes;
	;
	void draw ( VkCommandBuffer commandBuffer, Frame frame ) {

		VkBuffer vertexBuffers[] = { vertex.buffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline );
		vkCmdBindVertexBuffers ( commandBuffer, 0, 1, vertexBuffers, offsets );
		//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindIndexBuffer ( commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32 );
		
		vkCmdBindDescriptorSets ( commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[frame.index], 0, nullptr );
		vkCmdDrawIndexed ( commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0 );
	}

	void clean ( VkDevice device ) {
		vkDestroyPipeline ( device, pipeline, nullptr );
		vkDestroyPipelineLayout ( device, pipelineLayout, nullptr );


		vkDestroyBuffer ( device, indices.buffer, nullptr );
		vkFreeMemory ( device, indices.memory, nullptr );

		vkDestroyBuffer ( device, vertex.buffer, nullptr );
		vkFreeMemory ( device, vertex.memory, nullptr );
	}
};


namespace VKENGINE {

	struct UniformBufferObject
	{
		alignas(16) glm::mat4 model;
		alignas(16) glm::mat4 view;
		alignas(16) glm::mat4 proj;
	};

	class Entity
	{
	public:
		Device* device;
		VertexBuffer vertex;
		VertexBuffer indices;
		VkPipelineLayout pipelineLayout;
		VkPipeline pipeline;
		size_t indicesSizes;

		std::vector<VulkanObject3D> objects;
		

		void init ( Device* device, VkDescriptorPool descriptorPool, SEVIAN::Entity3D obj, std::vector<Frame> frames );
		void draw ( VkCommandBuffer commandBuffer, Frame frame );
		VkPipeline createGraphicsPipeline (
			VkVertexInputBindingDescription bindingDescription,
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions,
			VkDescriptorSetLayout& descriptorSetLayout );

		static VkVertexInputBindingDescription getBindingDescription () {
			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof ( SEVIAN::Vertex );
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}
		
		
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions () {
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions (4, {});

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof ( SEVIAN::Vertex, position );

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof ( SEVIAN::Vertex, normal );

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof ( SEVIAN::Vertex, texCoord );

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof ( SEVIAN::Vertex, color );

			return attributeDescriptions;
		}

		static VkVertexInputBindingDescription getBindingDescription3 () {
			VkVertexInputBindingDescription bindingDescription {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof ( SEVIAN::Vertex );
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}


		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions3 () {
			std::vector<VkVertexInputAttributeDescription> attributeDescriptions ( 4, {} );

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof ( SEVIAN::Vertex, position );

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof ( SEVIAN::Vertex, normal );

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof ( SEVIAN::Vertex, texCoord );

			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[3].offset = offsetof ( SEVIAN::Vertex, color );

			return attributeDescriptions;
		}

	private:

	};
}



