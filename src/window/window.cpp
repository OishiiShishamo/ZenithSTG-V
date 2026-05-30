#include "window/window.h"

#include <SDL3/SDL_video.h>
#include <memory>

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

#include "main.h"

namespace zenithstgv {
void initWindow(WindowPtr &window) {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
		SDL_Quit();
		throw std::runtime_error(SDL_GetError());
	}

	window.reset(SDL_CreateWindow(
	    kDefaultWindowTitle, kDefaultResolution.GetX(),
	    kDefaultResolution.GetY(), SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE));

	if (!window) {
		std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
		SDL_Quit();
		throw std::runtime_error(SDL_GetError());
	}

	SDL_SetWindowPosition(window.get(), SDL_WINDOWPOS_CENTERED,
	                      SDL_WINDOWPOS_CENTERED);
}
} // namespace zenithstgv
