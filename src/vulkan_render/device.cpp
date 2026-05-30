#include "vulkan_render/device.h"

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.h>

#include "main.h"
#include "vulkan_render/queue_family.h"
#include "vulkan_render/swapchain/swap_chain.h"

namespace zenithstgv {
void Device::pickPhysicalDevice(VkInstance instance,
                                VkPhysicalDevice &physical_device,
                                VkSurfaceKHR &surface) {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	int bestScore = 0;
	for (const auto &device : devices) {
		int score = rateDeviceSuitability(device, surface);
		if (score > bestScore) {
			bestScore = score;
			physical_device = device;
		}
	}

	if (physical_device == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Device::createLogicalDevice(VkDevice &device,
                                 VkPhysicalDevice &physical_device,
                                 VkQueue &graphics_queue,
                                 VkQueue &present_queue, VkSurfaceKHR surface) {
	QueueFamilyIndices indices = findQueueFamilies(physical_device, surface);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
	                                          indices.presentFamily.value()};

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

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount =
	    static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount =
	    static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (vkCreateDevice(physical_device, &createInfo, nullptr, &device) !=
	    VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0,
	                 &graphics_queue);
	vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &present_queue);
}

int Device::rateDeviceSuitability(VkPhysicalDevice device,
                                  VkSurfaceKHR &surface) {
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	QueueFamilyIndices indices = findQueueFamilies(device, surface);
	if (!indices.isComplete())
		return 0;

	int score = 0;

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		score += 1000;

	score += deviceProperties.limits.maxImageDimension2D;

	bool extensionsSupported = checkDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport =
		    SwapChain::querySwapChainSupport(device, surface);
		swapChainAdequate = !swapChainSupport.formats.empty() &&
		                    !swapChainSupport.presentModes.empty();
	}

	if (!extensionsSupported || !swapChainAdequate)
		return 0;
	return score;
}

bool Device::checkDeviceExtensionSupport(VkPhysicalDevice device) {
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
	                                     nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
	                                     availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(),
	                                         deviceExtensions.end());

	for (const auto &extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
} // namespace zenithstgv
