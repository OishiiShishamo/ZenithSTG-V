#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_SURFACE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_SURFACE_H_

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include "window/window.h"

namespace zenithstgv {
class Surface {
  public:
	static void createSurface(SDL_Window *window,
	                          const vk::UniqueInstance &instance,
	                          vk::SurfaceKHR &surface);
};
} // namespace zenithstgv

#endif
