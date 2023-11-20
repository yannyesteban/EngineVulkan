#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>


#include <stdexcept>
#include <cstddef>
#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <map>
#include <algorithm>
#include <unordered_map>



#include "db.h"
#include "PhysicalDevice.h"
#include "Device.h"
#include "Texture.h"
#include "Shader.h"
#include "Item.h"
#include "Actor.h"
#include "Frame.h"
#include "VulkanObject3D.h"



#define TINYOBJLOADER_IMPLEMENTATION
//#include "tiny_obj_loader.h"

#ifdef NDEBUG
const bool CenableValidationLayers = false;
#else
const bool CenableValidationLayers = true;
#endif
	//*****
struct QueueFamilyIndices1 {
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};
struct SwapChainSupportDetails1 {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct UniformBufferObject2 {
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};

struct Object3D {
	VertexBuffer vertex;
	VertexBuffer indices;
	Item item;

};

namespace std
{
	template<> struct hash<Data3D> {
		size_t operator()(Data3D const& vertex) const {
			return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
		}
	};
}

class VulkanEngine
{
public:
	GLFWwindow* window;

	std::vector<Texture> textures;
	std::vector<Shader> shaders;
	std::vector<Item> items;
	std::vector<Actor> actors;
	std::vector<Frame> frames;
	VkExtent2D extent;

	std::vector<Object3D> objects;

	std::vector<VulkanTexture> vkTextures;

	VkRenderPass renderPass;
	VkSampler textureSampler;


	//static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	
	void start();
	void draw();
	void drawFrame(Frame frame);
	//~VulkanEngine();
	void cleanup();
	void end();

	void loadModel(const char* path, std::vector<Data3D> vertices, std::vector<uint32_t> indices);
	
private:
	uint32_t currentFrame = 0;
	const int MAX_FRAMES_IN_FLIGHT = 2;
	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkSurfaceKHR surface;
	std::vector <PhysicalDeviceX>physicalDevices;
	PhysicalDeviceX * physicalDevice;
	SwapChain  swapChain;
	Device * device;
	VkDescriptorSetLayout descriptorSetLayout;
	VkDescriptorPool descriptorPool;
	VkCommandPool commandPool;
	VkPipelineLayout pipelineLayout;
	VkPipeline pipeline;

	VulkanDepthResources depthResources;

	std::vector<VulkanObject3D> objects3D;
	//****************************
	const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
	};

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
	const bool enableValidationLayers = CenableValidationLayers;
	//****************************
	

	bool checkValidationLayerSupport();
	std::vector<const char*> getRequiredExtensions();
	void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	void createInstance();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void setupDebugMessenger();
		
	void createSurface();
	QueueFamilyIndices1 findQueueFamilies(VkPhysicalDevice physicalDevice);
	bool checkDeviceExtensionSupport(VkPhysicalDevice physicalDevice);
	SwapChainSupportDetails1 querySwapChainSupport(VkPhysicalDevice physicalDevice);
	bool isDeviceSuitable(VkPhysicalDevice physicalDevice);
	std::vector <PhysicalDeviceX> getPhysicalDevices();
	
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

	void recreateSwapChain();

	void updateUniformBuffer(Frame frame);
	void recordCommandBuffer(Frame frame, VkCommandBuffer commandBuffer, uint32_t imageIndex);
	
	void cleanupSwapChain();
};

