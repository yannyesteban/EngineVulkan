#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
struct Frame
{
	VkDescriptorSet descriptorSet;
	VkFence inFlightFences;
	VkSemaphore imageAvailableSemaphores;
	VkSemaphore renderFinishedSemaphores;
	VkCommandBuffer commandBuffers;

	VkBuffer uniformBuffers;
	VkDeviceMemory uniformBuffersMemory;
	void* uniformBuffersMapped;
	const char* name;

	uint32_t index;
};

