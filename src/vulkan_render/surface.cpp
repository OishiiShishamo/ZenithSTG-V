#include "vulkan_render/surface.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "window/window.h"

namespace zenithstgv {
void Surface::createSurface(SDL_Window *window, VkInstance instance,
                            VkSurfaceKHR &surface) {
	if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
		throw std::runtime_error(SDL_GetError());
	}
}
} // namespace zenithstgv
