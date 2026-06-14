// miniaudio の実装を1つのTUにだけ含める
#define MINIAUDIO_IMPLEMENTATION
#include "3rdparty/miniaudio.h"

#include "sound_manager.h"

#include <iostream>
#include <unordered_map>
#include <vector>

namespace zenithstgv {
struct SoundManager::Impl {
	ma_engine engine{};
	bool initialized = false;

	std::unordered_map<std::string, std::string> se_paths;

	std::string bgm_current_id;
	ma_sound bgm_sound{};
	bool bgm_loaded = false;
};

SoundManager::SoundManager() : impl_(std::make_unique<Impl>()) {}

SoundManager::~SoundManager() { Shutdown(); }

void SoundManager::Init() {
	if (impl_->initialized)
		return;

	ma_engine_config cfg = ma_engine_config_init();
	if (ma_engine_init(&cfg, &impl_->engine) != MA_SUCCESS) {
		std::cerr << "[SoundManager] Failed to initialize miniaudio engine.\n";
		return;
	}

	impl_->initialized = true;

	std::cout << "[SoundManager] Initialized.\n";
	return;
}

void SoundManager::Shutdown() {
	if (!impl_->initialized)
		return;

	if (impl_->bgm_loaded) {
		ma_sound_stop(&impl_->bgm_sound);
		ma_sound_uninit(&impl_->bgm_sound);
		impl_->bgm_loaded = false;
	}

	ma_engine_uninit(&impl_->engine);
	impl_->initialized = false;
}

bool SoundManager::IsInitialized() const { return impl_->initialized; }

bool SoundManager::LoadSE(const std::string &id, const std::string &path) {
	if (!impl_->initialized)
		return false;
	impl_->se_paths[id] = path;
	return true;
}

void SoundManager::PlaySE(const std::string &id, float volume) {
	if (!impl_->initialized)
		return;

	auto it = impl_->se_paths.find(id);
	if (it == impl_->se_paths.end()) {
		std::cerr << "[SoundManager] SE not loaded: " << id << "\n";
		return;
	}

	ma_sound *snd = new ma_sound{};
	ma_uint32 flags = MA_SOUND_FLAG_ASYNC | MA_SOUND_FLAG_NO_SPATIALIZATION;
	if (ma_sound_init_from_file(&impl_->engine, it->second.c_str(), flags,
	                            nullptr, nullptr, snd) != MA_SUCCESS) {
		std::cerr << "[SoundManager] Failed to play SE: " << id << "\n";
		delete snd;
		return;
	}

	ma_sound_set_volume(snd, volume);

	ma_sound_set_end_callback(
	    snd,
	    [](void *, ma_sound *snd_ptr) {
		    ma_sound_uninit(snd_ptr);
		    delete snd_ptr;
	    },
	    nullptr);

	ma_sound_start(snd);
}

void SoundManager::UnloadSE(const std::string &id) {
	impl_->se_paths.erase(id);
}

bool SoundManager::LoadBGM(const std::string &id, const std::string &path) {
	if (!impl_->initialized)
		return false;
	if (impl_->bgm_current_id == id && impl_->bgm_loaded)
		return true;
	impl_->se_paths["__bgm_" + id] = path;
	return true;
}

void SoundManager::PlayBGM(const std::string &id, float volume, bool loop) {
	if (!impl_->initialized)
		return;

	if (impl_->bgm_loaded) {
		ma_sound_stop(&impl_->bgm_sound);
		ma_sound_uninit(&impl_->bgm_sound);
		impl_->bgm_loaded = false;
	}

	auto it = impl_->se_paths.find("__bgm_" + id);
	if (it == impl_->se_paths.end()) {
		std::cerr << "[SoundManager] BGM not registered: " << id
		          << " (call LoadBGM first)\n";
		return;
	}

	ma_uint32 flags = MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_NO_SPATIALIZATION;
	if (ma_sound_init_from_file(&impl_->engine, it->second.c_str(), flags,
	                            nullptr, nullptr,
	                            &impl_->bgm_sound) != MA_SUCCESS) {
		std::cerr << "[SoundManager] Failed to load BGM: " << id << "\n";
		return;
	}

	ma_sound_set_volume(&impl_->bgm_sound, volume);
	ma_sound_set_looping(&impl_->bgm_sound, loop ? MA_TRUE : MA_FALSE);
	ma_sound_start(&impl_->bgm_sound);

	impl_->bgm_loaded = true;
	impl_->bgm_current_id = id;
	std::cout << "[SoundManager] Playing BGM: " << id << "\n";
}

void SoundManager::StopBGM() {
	if (!impl_->initialized || !impl_->bgm_loaded)
		return;
	ma_sound_stop(&impl_->bgm_sound);
}

void SoundManager::FadeOutBGM(float duration_ms) {
	if (!impl_->initialized || !impl_->bgm_loaded)
		return;
	ma_sound_set_fade_in_milliseconds(&impl_->bgm_sound, -1.0f, 0.0f,
	                                  static_cast<ma_uint64>(duration_ms));
}

const std::string &SoundManager::CurrentBGM() const {
	return impl_->bgm_current_id;
}

void SoundManager::SetBGMVolume(float volume) {
	if (!impl_->initialized || !impl_->bgm_loaded)
		return;
	ma_sound_set_volume(&impl_->bgm_sound, volume);
}

void SoundManager::SetMasterVolume(float volume) {
	if (!impl_->initialized)
		return;
	ma_engine_set_volume(&impl_->engine, volume);
}

} // namespace zenithstgv
