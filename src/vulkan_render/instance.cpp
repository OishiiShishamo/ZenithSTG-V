#include "vulkan_render/instance.h"

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "main.h"
#include "vulkan/vulkan.hpp"

namespace zenithstgv {
void Instance::createInstance(vk::UniqueInstance &instance) {
	if (enableValidationLayers && !checkValidationLayerSupport())
		throw std::runtime_error(
		    "validation layers requested, but not available!");

	vk::ApplicationInfo appInfo{kGameName, VK_MAKE_VERSION(1, 0, 0),
	                            "ZenithSTG-V", VK_MAKE_VERSION(1, 0, 0),
	                            VK_API_VERSION_1_3};

	std::uint32_t extension_count = 0;

	const char *const *extensions =
	    SDL_Vulkan_GetInstanceExtensions(&extension_count);

	if (!extensions) {
		throw std::runtime_error(SDL_GetError());
	}

	vk::InstanceCreateInfo createInfo{
	    {},
	    &appInfo,
	    enableValidationLayers ? static_cast<uint32_t>(validationLayers.size())
	                           : 0,
	    enableValidationLayers ? validationLayers.data() : nullptr,
	    extension_count,
	    extensions};

	instance = vk::createInstanceUnique(createInfo);
}

bool Instance::checkValidationLayerSupport() {
	const auto availableLayers = vk::enumerateInstanceLayerProperties();

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
