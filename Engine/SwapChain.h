#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
class SwapChain
{
public:
	VkDevice device;
	VkSwapchainKHR swapchain;
	std::vector<VkImage> swapChainImages;
	VkRenderPass renderPass;
	VkFormat imageFormat;
	VkExtent2D extent;
	std::vector<VkImageView> imageViews;
	std::vector<VkFramebuffer> framebuffers;

	~SwapChain();
	void createRenderPass();
};

