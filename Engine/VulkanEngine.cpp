#include "VulkanEngine.h"




static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
	std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

void VulkanEngine::cleanupSwapChain() {
	
	vkDestroyImageView(device->device, depthResources.imageView, nullptr);
	vkDestroyImage(device->device, depthResources.image, nullptr);
	vkFreeMemory(device->device, depthResources.imageMemory, nullptr);
	
	for (auto framebuffer : swapChain.framebuffers) {
		vkDestroyFramebuffer(device->device, framebuffer, nullptr);
	}

	for (auto imageView : swapChain.imageViews) {
		vkDestroyImageView(device->device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(device->device, swapChain.swapchain, nullptr);
}

void VulkanEngine::cleanup()
{

	cleanupSwapChain();
	
	//vkDestroyPipeline(device->device, pipeline, nullptr);
	//vkDestroyPipelineLayout(device->device, device->pipelineLayout, nullptr);

	for (auto& o : objects3D) {
		o.clean(device->device);
	}

	vkDestroyRenderPass(device->device, renderPass, nullptr);

	for (auto& frame : frames) { 
		vkDestroyBuffer(device->device, frame.uniformBuffers, nullptr);
		vkFreeMemory(device->device, frame.uniformBuffersMemory, nullptr);
	
	}
	
	vkDestroyDescriptorPool(device->device, descriptorPool, nullptr);

	for (auto& o : vkTextures) { 
		vkDestroySampler(device->device, o.sampler, nullptr);
		vkDestroyImageView(device->device, o.imageView, nullptr);

		vkDestroyImage(device->device, o.textureImage, nullptr);
		vkFreeMemory(device->device, o.textureImageMemory, nullptr);
	}



	
	vkDestroyDescriptorSetLayout(device->device, device->descriptorSetLayout, nullptr);

	for (auto& o : objects) {

		vkDestroyBuffer(device->device, o.indices.buffer, nullptr);
		vkFreeMemory(device->device, o.indices.memory, nullptr);

		vkDestroyBuffer(device->device, o.vertex.buffer, nullptr);
		vkFreeMemory(device->device, o.vertex.memory, nullptr);
		
	}

	for (auto& frame : frames) {
		vkDestroySemaphore(device->device, frame.renderFinishedSemaphores, nullptr);
		vkDestroySemaphore(device->device, frame.imageAvailableSemaphores, nullptr);
		vkDestroyFence(device->device, frame.inFlightFences, nullptr);
	}

	


	vkDestroyCommandPool(device->device, device->commandPool, nullptr);
	vkDestroyDevice(device->device, nullptr);
	if (enableValidationLayers) {
		DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
	}
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
	db("vkDestroyInstance");

	glfwDestroyWindow(window);

	glfwTerminate();
}
void VulkanEngine::start()
{
	db("start");
	createInstance();
	setupDebugMessenger();
	createSurface();
	physicalDevices = getPhysicalDevices();
	
	int position = 0;
	
	device = physicalDevices.at(position).createDevice();
	commandPool = physicalDevices.at(position).createCommandPool();

	graphicsQueue = physicalDevices.at(position).graphicsQueue;
	presentQueue = physicalDevices.at(position).presentQueue;
	
	device->commandPool = commandPool;
	

	swapChain = physicalDevices.at(position).createSwapChain(window);
	extent = swapChain.extent;
	renderPass = device->createRenderPass(swapChain.imageFormat);
	
	
	depthResources = device->createDepthResources(extent);

	swapChain.framebuffers = physicalDevices.at(position).createFramebuffers(swapChain, renderPass, { depthResources.imageView });
	

	device->renderPass = renderPass;
	//pipelineLayout = device->pipelineLayout;
	
	descriptorSetLayout = device->createDescriptorSetLayout();
	descriptorPool = device->createDescriptorPool();
	device->descriptorSetLayout = descriptorSetLayout;
	
	std::map<std::string, VulkanTexture> mTextures;


	for (auto& t : textures) {

		VulkanTexture vt = device->createTexture(t.source);
		vt.id = "yanny";
		vkTextures.push_back(vt);
		mTextures[t.id] = vt;
	}

	


	frames.resize(2);
	
	uint32_t currentFrame = 0;
	for (auto& frame : frames) {
		db("creando un frame");
		frame.index = currentFrame;
		device->createUniformBuffers(frame, sizeof(UniformBufferObject2));
		device->createSyncObjects(frame);
		currentFrame++;
	}

	
	device->createCommandBuffers(frames, device->commandPool);
	
	

	

	//device->createTexture("textures/viking_room.png");
	//textureSampler = device->createTextureSampler();
	
	
	VkVertexInputBindingDescription bindingDescription{};
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Data3D);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
	attributeDescriptions.resize(3);

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Data3D, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Data3D, color);

	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Data3D, texCoord);

	
	

	for (auto& i : items) {
		VulkanObject3D obj;
		
		auto vertices = i.data;
		auto indices = i.indices;
		
		obj.descriptorSets = device->createDescriptorSets(frames, descriptorPool, mTextures[i.texture].imageView, mTextures[i.texture].sampler, sizeof(UniformBufferObject2));

		auto pipeline = device->createGraphicsPipeline(bindingDescription, attributeDescriptions, descriptorSetLayout);
		auto pipelineLayout = device->pipelineLayout;

		obj.vertex = device->createDataBuffer((void*)vertices.data(), sizeof(vertices[0]) * vertices.size(), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);;
		obj.indices = device->createDataBuffer((void*)indices.data(), sizeof(indices[0]) * indices.size(), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		obj.indicesSizes = indices.size();
		obj.pipeline = pipeline;
		obj.pipelineLayout = pipelineLayout;

		objects3D.push_back(obj);
	}


}

bool VulkanEngine::checkValidationLayerSupport()

{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

std::vector<const char*> VulkanEngine::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

void VulkanEngine::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}

void VulkanEngine::createInstance() {
	if (enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("validation layers requested, but not available!");
	}

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Triangle";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "No Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	auto extensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (enableValidationLayers) {
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		populateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

VkResult VulkanEngine::CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}



void VulkanEngine::setupDebugMessenger() {
	if (!enableValidationLayers) return;

	VkDebugUtilsMessengerCreateInfoEXT createInfo;
	populateDebugMessengerCreateInfo(createInfo);

	if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
		throw std::runtime_error("failed to set up debug messenger!");
	}
}
void VulkanEngine::createSurface() {
	db("xxxxxxxxxxxxxx");
	if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}
QueueFamilyIndices1 VulkanEngine::findQueueFamilies(VkPhysicalDevice physicalDevice) {
	QueueFamilyIndices1 indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.isComplete()) {
			break;
		}

		i++;
	}

	return indices;
}



bool VulkanEngine::checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
SwapChainSupportDetails1 VulkanEngine::querySwapChainSupport(VkPhysicalDevice physicalDevice) {
	SwapChainSupportDetails1 details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

bool VulkanEngine::isDeviceSuitable(VkPhysicalDevice physicalDevice) {
	QueueFamilyIndices1 indices = findQueueFamilies(physicalDevice);

	bool extensionsSupported = checkDeviceExtensionSupport(physicalDevice);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails1 swapChainSupport = querySwapChainSupport(physicalDevice);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

std::vector <PhysicalDeviceX> VulkanEngine::getPhysicalDevices() {

	std::vector <PhysicalDeviceX>physicalDevices;

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());


	for (const auto& device : devices) {
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		std::cout << "deviceProperties:  " << deviceProperties.deviceName << std::endl;
		if (isDeviceSuitable(device)) {

			PhysicalDeviceX ph = PhysicalDeviceX(instance, surface, device);
			physicalDevices.push_back(ph);
			//physicalDevice = device;
			//break;
		}
	}

	if (physicalDevices.size() == 0) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}

	return physicalDevices;
}

void VulkanEngine::DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator) {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(instance, debugMessenger, pAllocator);
	}
}

void VulkanEngine::recreateSwapChain() {}

void VulkanEngine::draw() {

	drawFrame(frames[currentFrame]);
	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanEngine::updateUniformBuffer(Frame frame) {
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObject2 ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), extent.width / (float)extent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	memcpy(frame.uniformBuffersMapped, &ubo, sizeof(ubo));
}

void VulkanEngine::recordCommandBuffer(Frame frame, VkCommandBuffer commandBuffer, uint32_t imageIndex) {
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = device->renderPass;
	renderPassInfo.framebuffer = swapChain.framebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain.extent;

	/*VkClearValue clearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	*/

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChain.extent.width;
	viewport.height = (float)swapChain.extent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain.extent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	for (auto& object : objects3D) {
		object.draw(commandBuffer, frame);
	}
	/*

	VkBuffer vertexBuffers[] = { objects[0].vertex.buffer };
	VkDeviceSize offsets[] = { 0 };

	auto obj = objects[0];

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, objects[0].indices.buffer, 0, VK_INDEX_TYPE_UINT16);
	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1,&frame.descriptorSet, 0, nullptr);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(obj.item.indices.size()), 1, 0, 0, 0);
	*/


	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}
}

void VulkanEngine::drawFrame(Frame frame) {
	//VkDevice device;
	//VkSwapchainKHR swapChain;
	bool framebufferResized = false;

	vkWaitForFences(device->device, 1, &frame.inFlightFences, VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(device->device, swapChain.swapchain, UINT64_MAX, frame.imageAvailableSemaphores, VK_NULL_HANDLE, &imageIndex);
	
	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	updateUniformBuffer(frame);

	vkResetFences(device->device, 1, &frame.inFlightFences);

	vkResetCommandBuffer(frame.commandBuffers, /*VkCommandBufferResetFlagBits*/ 0);
	recordCommandBuffer(frame, frame.commandBuffers, imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { frame.imageAvailableSemaphores };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &frame.commandBuffers;

	VkSemaphore signalSemaphores[] = { frame.renderFinishedSemaphores };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, frame.inFlightFences) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapChain.swapchain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

void VulkanEngine::end() {
	vkDeviceWaitIdle(device->device);
}

void  VulkanEngine::loadModel(const char * path, std::vector<Data3D> vertices, std::vector<uint32_t> indices) {
	
	

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path)) {
		throw std::runtime_error(warn + err);
	}
	
	std::unordered_map<Data3D, uint32_t> uniqueVertices{};
	
	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Data3D vertex{};
			
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
			
			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			
			indices.push_back(uniqueVertices[vertex]);
			
		}
	}
	
}
