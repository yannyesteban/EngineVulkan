#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "tiny_obj_loader.h"
#include "Device.h"

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
	void draw(VkCommandBuffer commandBuffer, Frame frame) {

		VkBuffer vertexBuffers[] = { vertex.buffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
		//vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindIndexBuffer(commandBuffer, indices.buffer, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[frame.index], 0, nullptr);
		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indicesSizes), 1, 0, 0, 0);
	}

	void clean(VkDevice device) {
		vkDestroyPipeline(device, pipeline, nullptr);
		vkDestroyPipelineLayout(device, pipelineLayout, nullptr);


		vkDestroyBuffer(device, indices.buffer, nullptr);
		vkFreeMemory(device, indices.memory, nullptr);

		vkDestroyBuffer(device, vertex.buffer, nullptr);
		vkFreeMemory(device, vertex.memory, nullptr);
	}
};

