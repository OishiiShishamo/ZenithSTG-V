#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_KEYBOARD_H_
#define ZENITHSTGV_INCLUDE_UTILITY_KEYBOARD_H_

#include <unordered_map>
#include <vector>

#include <SDL3/SDL.h>

namespace zenithstgv {
class Keyboard {
  public:
	void Update() {
		prev_keys_ = curr_keys_;
		const bool *state = SDL_GetKeyboardState(nullptr);
		curr_keys_.clear();
		for (auto key : watched_keys_) {
			curr_keys_[key] = state[SDL_GetScancodeFromKey(key, nullptr)];
		}
	}

	bool IsHeld(SDL_Keycode key) const {
		auto it = curr_keys_.find(key);
		return it != curr_keys_.end() && it->second;
	}

	bool IsPressed(SDL_Keycode key) const {
		auto it = curr_keys_.find(key);
		auto prev = prev_keys_.find(key);
		return (it != curr_keys_.end() && it->second) &&
		       (prev == prev_keys_.end() || !prev->second);
	}

	bool IsReleased(SDL_Keycode key) const {
		auto it = curr_keys_.find(key);
		auto prev = prev_keys_.find(key);
		return (it == curr_keys_.end() || !it->second) &&
		       (prev != prev_keys_.end() && prev->second);
	}

	void Watch(SDL_Keycode key) { watched_keys_.push_back(key); }

  private:
	std::vector<SDL_Keycode> watched_keys_;
	std::unordered_map<SDL_Keycode, bool> curr_keys_;
	std::unordered_map<SDL_Keycode, bool> prev_keys_;
};

void InitKeyboard(Keyboard &kb);
} // namespace zenithstgv

#endif
