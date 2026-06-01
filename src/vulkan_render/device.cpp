#include "vulkan_render/device.h"

#include <cstdint>
#include <set>
#include <stdexcept>
#include <vector>

#include <vulkan/vulkan.hpp>

#include "main.h"
#include "vulkan_render/queue_family.h"
#include "vulkan_render/swapchain/swap_chain.h"

namespace zenithstgv {
void Device::pickPhysicalDevice(vk::Instance instance,
                                vk::PhysicalDevice &physicalDevice,
                                vk::SurfaceKHR surface) {
	const auto devices = instance.enumeratePhysicalDevices();

	if (devices.empty()) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	int bestScore = 0;

	for (const auto &device : devices) {
		const int score = rateDeviceSuitability(device, surface);

		if (score > bestScore) {
			bestScore = score;
			physicalDevice = device;
		}
	}

	if (!physicalDevice) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void Device::createLogicalDevice(vk::UniqueDevice &device,
                                 vk::PhysicalDevice physicalDevice,
                                 vk::Queue &graphicsQueue,
                                 vk::Queue &presentQueue,
                                 vk::SurfaceKHR surface) {
	const QueueFamilyIndices indices =
	    findQueueFamilies(physicalDevice, surface);

	std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
	                                          indices.presentFamily.value()};

	float queuePriority = 1.0f;

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

	for (uint32_t queueFamily : uniqueQueueFamilies) {
		queueCreateInfos.emplace_back(vk::DeviceQueueCreateFlags{}, queueFamily,
		                              1, &queuePriority);
	}

	vk::PhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;

	vk::DeviceCreateInfo createInfo{
	    {},
	    static_cast<uint32_t>(queueCreateInfos.size()),
	    queueCreateInfos.data(),
	    0,
	    nullptr,
	    static_cast<uint32_t>(deviceExtensions.size()),
	    deviceExtensions.data(),
	    &deviceFeatures};

	device = physicalDevice.createDeviceUnique(createInfo);

	graphicsQueue = device->getQueue(indices.graphicsFamily.value(), 0);

	presentQueue = device->getQueue(indices.presentFamily.value(), 0);
}

int Device::rateDeviceSuitability(vk::PhysicalDevice device,
                                  vk::SurfaceKHR surface) {
	const auto deviceProperties = device.getProperties();

	const auto deviceFeatures = device.getFeatures();

	const QueueFamilyIndices indices = findQueueFamilies(device, surface);

	if (!indices.isComplete()) {
		return 0;
	}

	int score = 0;

	if (deviceProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu) {
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;

	const bool extensionsSupported = checkDeviceExtensionSupport(device);

	if (!deviceFeatures.samplerAnisotropy) {
		return 0;
	}

	bool swapChainAdequate = false;

	if (extensionsSupported) {
		const auto swapChainSupport =
		    SwapChain::querySwapChainSupport(device, surface);

		swapChainAdequate = !swapChainSupport.formats.empty() &&
		                    !swapChainSupport.presentModes.empty();
	}

	if (!extensionsSupported || !swapChainAdequate) {
		return 0;
	}

	return score;
}

bool Device::checkDeviceExtensionSupport(vk::PhysicalDevice device) {
	const auto availableExtensions =
	    device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(),
	                                         deviceExtensions.end());

	for (const auto &extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}
} // namespace zenithstgv
