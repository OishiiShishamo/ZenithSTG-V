#include "utility/keyboard.h"

namespace zenithstgv {
void InitKeyboard(Keyboard &kb) {
	kb.Watch(SDLK_RIGHT);
	kb.Watch(SDLK_LEFT);
	kb.Watch(SDLK_UP);
	kb.Watch(SDLK_DOWN);

	kb.Watch(SDLK_LSHIFT);
	kb.Watch(SDLK_Z);
	kb.Watch(SDLK_X);
	kb.Watch(SDLK_C);
}
} // namespace zenithstgv
