#include "vulkan_render/surface.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "window/window.h"

namespace zenithstgv {
void Surface::createSurface(SDL_Window *window,
                            const vk::UniqueInstance &instance,
                            vk::UniqueSurfaceKHR &surface) {
	VkSurfaceKHR raw{};

	if (!SDL_Vulkan_CreateSurface(
	        window, static_cast<VkInstance>(instance.get()), nullptr, &raw)) {
		throw std::runtime_error(SDL_GetError());
	}

	surface = vk::UniqueSurfaceKHR(
	    vk::SurfaceKHR(raw),
	    vk::detail::ObjectDestroy<vk::Instance,
	                              VULKAN_HPP_DEFAULT_DISPATCHER_TYPE>(
	        instance.get()));
}
} // namespace zenithstgv
