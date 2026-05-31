#pragma once
#ifndef ZENITHSTGV_INCLUDE_VULKAN_RENDER_INSTANCE_H_
#define ZENITHSTGV_INCLUDE_VULKAN_RENDER_INSTANCE_H_

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include "main.h"

namespace zenithstgv {
class Instance {
  public:
	static void createInstance(VkInstance &instance);

  private:
	static bool checkValidationLayerSupport();
};
} // namespace zenithstgv
#endif
