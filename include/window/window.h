#pragma once
#ifndef ZENITHSTGV_SRC_WINDOW_WINDOW_H_
#define ZENITHSTGV_SRC_WINDOW_WINDOW_H_

#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "main.h"

namespace zenithstgv {
struct SDL_Deleter {
	void operator()(SDL_Window *window) const {
		if (window) {
			SDL_DestroyWindow(window);
		}
	}
};

using WindowPtr = std::unique_ptr<SDL_Window, SDL_Deleter>;

void initWindow(WindowPtr &window);
} // namespace zenithstgv

#endif
