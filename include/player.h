#pragma once
#ifndef ZENITHSTGV_INCLUDE_PLAYER_H_
#define ZENITHSTGV_INCLUDE_PLAYER_H_

#include "main.h"

#include <array>
#include <atomic>

#include "color.h"
#include "easing.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "utility/keyboard.h"
#include "vec2d.h"

#define kPlayerDefaultX kCenterX
#define kPlayerDefaultY 900
#define kPlayerDefaultPos Vec2D(kPlayerDefaultX, kPlayerDefaultY)
#define kDefaultLife 2
#define kDefaultBomb 3
#define kPlayerCol 0.0f
#define kPlayerProtect 60

#define kPlayerSideBorderOffset 16
#define kPlayerTopBorderOffset 24
#define kPlayerBottomBorderOffset 32

#define kPlayerNum 1

namespace zenithstgv {
enum PlayerFlags {
	kIsFocus = 1 << 0,
	kIsShowCol = 1 << 1,
	kIsProtect = 1 << 2,
};

class Player {
  public:
	std::atomic<std::uint8_t> flags_ = 0b00000000;
	int default_life_ = kDefaultLife;
	std::atomic<int> life_ = default_life_;
	int default_bomb_ = kDefaultBomb;
	std::atomic<int> bomb_ = default_bomb_;
	double col_size_ = 0.0f;
	double slow_ = 3;
	double fast_ = 5;
	// 無敵時間 / Invincibility time.
	int protect_ = kPlayerProtect;
	// 残り無敵時間 / Remaining invincibility time.
	std::atomic<int> protect_time_ = 0;

	// ゲッターとセッター / Getters and Setters.
	Vec2D GetPos();
	void SetPos(const Vec2D &v);
	bool GetIsFocus(std::memory_order order = std::memory_order_seq_cst);
	void SetIsFocus(bool b,
	                std::memory_order order = std::memory_order_seq_cst);
	bool GetIsShowCol(std::memory_order order = std::memory_order_seq_cst);
	bool GetIsProtect(std::memory_order order = std::memory_order_seq_cst);
	void SetIsProtect(bool b,
	                  std::memory_order order = std::memory_order_seq_cst);

	double AimPlayer(const Vec2D &v) const;
	double RangePlayer(const Vec2D &v);
	void MovePlayer(const Keyboard &kb);
	void RenderPlayer(const AtlasBuilder &atlas,
	                  std::array<std::vector<InstanceData>, 5> &instance_lists);
	void Shot(const Keyboard &kb);
	bool TryHit();
	void HitPlayer();
	void RoutinePlayer(const Keyboard &kb);

  private:
	Vec2D pos_ = kPlayerDefaultPos;
	Vec2D vec_ = Vec2D(0, 0);
};
} // namespace zenithstgv

#endif
