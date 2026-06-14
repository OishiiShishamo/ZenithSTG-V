#pragma once
#ifndef ZENITHSTGV_INCLUDE_SOUND_MANAGER_H_
#define ZENITHSTGV_INCLUDE_SOUND_MANAGER_H_

#include <memory>
#include <string>
#include <unordered_map>

namespace zenithstgv {
class SoundManager {
  public:
	SoundManager();
	~SoundManager();

	void Init();

	bool LoadSE(const std::string &id, const std::string &path);
	void PlaySE(const std::string &id, float volume = 1.0f);
	void UnloadSE(const std::string &id);

	bool LoadBGM(const std::string &id, const std::string &path);
	void PlayBGM(const std::string &id, float volume = 1.0f, bool loop = true);
	void StopBGM();
	void FadeOutBGM(float duration_ms = 1000.0f);
	const std::string &CurrentBGM() const;
	void SetBGMVolume(float volume);
	void SetMasterVolume(float volume);

	bool IsInitialized() const;

	void Shutdown();

  private:
	struct Impl;
	std::unique_ptr<Impl> impl_;
};

} // namespace zenithstgv

#endif
