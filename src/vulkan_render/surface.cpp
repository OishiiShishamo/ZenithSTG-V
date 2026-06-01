#include "vulkan_render/surface.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "window/window.h"

namespace zenithstgv {
void Surface::createSurface(SDL_Window *window,
                            const vk::UniqueInstance &instance,
                            vk::SurfaceKHR &surface) {
	VkSurfaceKHR rawSurface{};

	if (!SDL_Vulkan_CreateSurface(window,
	                              static_cast<VkInstance>(instance.get()),
	                              nullptr, &rawSurface)) {
		throw std::runtime_error(SDL_GetError());
	}

	surface = rawSurface;
}
} // namespace zenithstgv
