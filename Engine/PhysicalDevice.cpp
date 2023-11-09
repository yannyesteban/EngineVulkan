#include "PhysicalDevice.h"



PhysicalDeviceX::PhysicalDeviceX(VkInstance _instance, VkSurfaceKHR _surface, VkPhysicalDevice _physicalDevice)
{
	instance = _instance;
	surface = _surface;
	physicalDevice = _physicalDevice;

}

void PhysicalDeviceX::findQueueFamilies() {


	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport) {
			presentFamily = i;
		}

		if (isComplete()) {
			break;
		}

		i++;
	}


}

VkImageView PhysicalDeviceX::createImageView(VkImage image, VkFormat format) {
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void PhysicalDeviceX::createLogicalDevice() {
	//QueueFamilyIndices indices = 
	findQueueFamilies();
	
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	
	std::set<uint32_t> uniqueQueueFamilies = { graphicsFamily.value(), presentFamily.value() };
	
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}
	
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		createInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}
	printf("que es un device---> (%p)\n", device);
	vkGetDeviceQueue(device, graphicsFamily.value(), 0, &graphicsQueue);
	vkGetDeviceQueue(device, presentFamily.value(), 0, &presentQueue);
}

void PhysicalDeviceX::querySwapChainSupport() {
	

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
	}

	
}

SwapChain PhysicalDeviceX::createSwapChain(GLFWwindow* window) {
	
	
	//SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
	querySwapChainSupport();
	VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(formats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(presentModes);
	VkExtent2D extent = chooseSwapExtent(capabilities, window);

	uint32_t imageCount = capabilities.minImageCount + 1;
	if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
		imageCount = capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	findQueueFamilies();
	uint32_t queueFamilyIndices[] = { graphicsFamily.value(), presentFamily.value() };

	if (graphicsFamily != presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	SwapChain swapChain;
	

	//VkSwapchainKHR swapChain;
	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &swapChain.swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapChain.swapchain, &imageCount, nullptr);
	swapChain.swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain.swapchain, &imageCount, swapChain.swapChainImages.data());

	swapChain.imageFormat = surfaceFormat.format;
	swapChain.extent = extent;
	swapChain.device = device;
	swapChain.imageViews.resize(swapChain.swapChainImages.size());

	for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
		swapChain.imageViews[i] = createImageView(swapChain.swapChainImages[i], swapChain.imageFormat);
	}


	return swapChain;
	
}

std::vector<VkFramebuffer> PhysicalDeviceX::createFramebuffers(SwapChain swapChain, VkRenderPass renderPass, std::vector<VkImageView> attachments) {

	std::vector<VkFramebuffer> swapChainFramebuffers;


	swapChainFramebuffers.resize(swapChain.imageViews.size());

	for (size_t i = 0; i < swapChain.imageViews.size(); i++) {
		

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		framebufferInfo.pAttachments = attachments.data();
		framebufferInfo.width = swapChain.extent.width;
		framebufferInfo.height = swapChain.extent.height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to create framebuffer!");
		}
	}

	return swapChainFramebuffers;
}


VkCommandPool PhysicalDeviceX::createCommandPool() {

	VkCommandPool commandPool;


	//QueueFamilyIndices queueFamilyIndices =
		findQueueFamilies();

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = graphicsFamily.value();

	if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics command pool!");
	}

	return commandPool;
}

Device * PhysicalDeviceX::createDevice()
{
	createLogicalDevice();
	Device  * devic =  new Device(physicalDevice, device, graphicsQueue);
	return devic;
}
