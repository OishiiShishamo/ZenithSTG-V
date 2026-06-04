#pragma once
#ifndef ZENITHSTGV_INCLUDE_MAIN_H_
#define ZENITHSTGV_INCLUDE_MAIN_H_

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <vulkan/vulkan.h>

#include "vec2d.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

inline constexpr const char *kGameName = "ZenithSTG-V";
inline constexpr const char *kDefaultWindowTitle = "ZenithSTG-V";
inline const zenithstgv::Vec2D kDefaultResolution(1600, 900);

inline constexpr uint32_t kMaxInstances = 65536;

inline constexpr uint32_t kMaxBullet = 16384;
inline constexpr uint32_t kMaxLaser = 8192;

#define kCenterX 960
#define kCenterY 540
#define kCenter Vec2D(kCenterX, kCenterY)

inline constexpr int kBorderLeft = 0;     // 528
inline constexpr int kBorderRight = 1920; // 1392
inline constexpr int kBorderUp = 0;       // 35
inline constexpr int kBorderDown = 1080;  // 1045

inline constexpr int kGrazeEnabled = 1;
inline constexpr int kBulletGrazeEveryFrame = 0;
inline constexpr int kLaserGrazeEveryFrame = 1;

inline constexpr int kMaxFramesInFlight = 2;

const std::vector<const char *> validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#ifdef NDEBUG
inline constexpr bool enableValidationLayers = false;
#else
inline constexpr bool enableValidationLayers = true;
#endif

namespace zenithstgv {}

#endif
