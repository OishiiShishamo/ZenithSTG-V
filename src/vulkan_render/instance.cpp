#include "vulkan_render/instance.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "main.h"

namespace zenithstgv {
void Instance::createInstance(VkInstance &instance) {
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error(
		    "validation layers requested, but not available!");

	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = kGameName;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "ZenithSTG-V";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_3;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::uint32_t extension_count = 0;

	const char *const *extensions =
	    SDL_Vulkan_GetInstanceExtensions(&extension_count);

	if (!extensions) {
		std::cerr << "SDL_Vulkan_GetInstanceExtensions failed: "
		          << SDL_GetError() << '\n';

		SDL_Quit();
		throw std::runtime_error(SDL_GetError());
	}

	createInfo.enabledExtensionCount = extension_count;
	createInfo.ppEnabledExtensionNames = extensions;

	if (enableValidationLayers) {
		createInfo.enabledLayerCount =
		    static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		createInfo.pNext = nullptr;
	} else {
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("failed to create instance!");
	}
}

bool Instance::checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char *layerName : validationLayers) {
		bool layerFound = false;

		for (const auto &layerProperties : availableLayers) {
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
} // namespace zenithstgv
