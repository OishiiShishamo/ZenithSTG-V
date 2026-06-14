#include "player.h"

#include <array>
#include <atomic>

#include "color.h"
#include "easing.h"
#include "instance_data.h"
#include "utility/atlas_builder.h"
#include "utility/keyboard.h"
#include "vec2d.h"

// TODO: もっと真面目に作る
// TODO: コントローラーに対応させる

namespace zenithstgv {
// ゲッターとセッター / Getters and Setters.
Vec2D Player::GetPos() { return pos_; }

void Player::SetPos(const Vec2D &v) { pos_ = v; }

bool Player::GetIsFocus(std::memory_order order) {
	return flags_.load(order) & kIsFocus;
}

void Player::SetIsFocus(bool b, std::memory_order order) {
	if (b) {
		flags_.fetch_or(kIsFocus, order);
	} else {
		flags_.fetch_and(~kIsFocus, order);
	}
}

bool Player::GetIsShowCol(std::memory_order order) {
	return flags_.load(order) & kIsShowCol;
}

bool Player::GetIsProtect(std::memory_order order) {
	return flags_.load(order) & kIsProtect;
}

void Player::SetIsProtect(bool b, std::memory_order order) {
	if (b) {
		flags_.fetch_or(kIsProtect, order);
	} else {
		flags_.fetch_and(~kIsProtect, order);
	}
}

double Player::AimPlayer(const Vec2D &v) const { return Vec2DToAngle(pos_, v); }

double Player::RangePlayer(const Vec2D &v) { return Range(v, pos_); }

void Player::MovePlayer(const Keyboard &kb) {
	vec_ = Vec2D(_mm_set1_pd(0));
	if (kb.IsHeld(SDLK_RIGHT))
		vec_ += Vec2D(1, 0);
	if (kb.IsHeld(SDLK_LEFT))
		vec_ -= Vec2D(1, 0);
	if (kb.IsHeld(SDLK_UP))
		vec_ -= Vec2D(0, 1);
	if (kb.IsHeld(SDLK_DOWN))
		vec_ += Vec2D(0, 1);

	vec_.Normalize();

	if (kb.IsHeld(SDLK_LSHIFT)) {
		pos_ += vec_ * slow_;
	} else {
		pos_ += vec_ * fast_;
	}
	if (pos_.GetX() < kBorderLeft + kPlayerSideBorderOffset)
		pos_.SetX(kBorderLeft + kPlayerSideBorderOffset);
	if (pos_.GetX() > kBorderRight - kPlayerSideBorderOffset)
		pos_.SetX(kBorderRight - kPlayerSideBorderOffset);
	if (pos_.GetY() > kBorderDown - kPlayerBottomBorderOffset)
		pos_.SetY(kBorderDown - kPlayerBottomBorderOffset);
	if (pos_.GetY() < kBorderUp + kPlayerTopBorderOffset)
		pos_.SetY(kBorderUp + kPlayerTopBorderOffset);
}

void Player::RenderPlayer(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 5> &instance_lists) {
	std::array<Vec2D, 4> world;
	int half_x = 128 / 2;
	int half_y = 128 / 2;
	std::array<Vec2D, 4> local = {Vec2D(-half_x, -half_y),
	                              Vec2D(half_x, -half_y), Vec2D(half_x, half_y),
	                              Vec2D(-half_x, half_y)};
	for (int i = 0; i < 4; ++i) {
		world[i] = pos_ + local[i];
	}
	Sprite spr;
	spr.pos[0] = world[0].ToNDC(ScreenSize(1920, 1080));
	spr.pos[1] = world[1].ToNDC(ScreenSize(1920, 1080));
	spr.pos[2] = world[2].ToNDC(ScreenSize(1920, 1080));
	spr.pos[3] = world[3].ToNDC(ScreenSize(1920, 1080));
	spr.color = Color(1.0, 1.0, 1.0, 1.0);
	spr.blend_mode = BlendMode::kNormal;
	spr.uv_key = "player";
	instance_lists[0].push_back(
	    spr.toInstanceData(atlas.getEntries().at(spr.uv_key)));
}

void Player::Shot(const Keyboard &kb) {
	if (kb.IsHeld(SDLK_Z)) {
		// CreatePlayerShot(pos_, Color(255, 255, 255), 0, kBlendAdd, 255, 1,
		// 24,
		//                  24, 0, 0, 1.0f, 1.0f, 0, 0, Rad(90), Rad(90), 0, 0,
		//                  0, 50, kEaseInCubic, 60, -1, 0);
	}
}

bool Player::TryHit() {
	uint8_t expected = flags_.load(std::memory_order_relaxed);
	while (!(expected & kIsProtect)) {
		uint8_t desired = expected | kIsProtect;
		if (flags_.compare_exchange_weak(expected, desired,
		                                 std::memory_order_acq_rel,
		                                 std::memory_order_relaxed)) {
			return true;
		}
	}
	return false;
}

void Player::HitPlayer() {
	if (!TryHit())
		return;
	SetIsProtect(1);
	for (int i = 0; i < 64; i++) {
		// double angle = Rad(static_cast<double>(rng() % 36000) / 100.0);
		// CreateParticle(pos_, Color(1.0f, 0.0f, 0.0f),
		//                ParticleType::kParticleRect, BlendType::kBlendAdd,
		//                255, EaseType::kEaseInQuad, 60, 0, 0, 0, 0, 0, 0.5f,
		//                0.0f, EaseType::kEaseInQuad, 60, 0, angle, angle, 0,
		//                0, static_cast<double>(rng() % 160) / 10.0f + 3, 0.0f,
		//                EaseType::kEaseInQuad, 60);
	}
	protect_time_.store(kPlayerProtect);
	pos_ = kPlayerDefaultPos;
	life_.fetch_sub(1, std::memory_order_relaxed);
	bomb_ = default_bomb_;
	// sound_mng_.ReserveSe(kSoundEffectPlayerHit);
}

void Player::RoutinePlayer(const Keyboard &kb) {
	MovePlayer(kb);
	Shot(kb);
	protect_time_.fetch_sub(1, std::memory_order_relaxed);
	if (protect_time_.load() <= 0) {
		SetIsProtect(0);
	}
}
} // namespace zenithstgv
