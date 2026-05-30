#pragma once
#ifndef ZENITHSTGV_SRC_GLOBAL_H_
#define ZENITHSTGV_SRC_GLOBAL_H_

#include <array>

namespace zenithstgv {
	inline constexpr double kPi = 3.1415926535897932384626433832795;
	inline constexpr double kTau = kPi * 2;
	inline constexpr double kOneDeg = (kPi / 180);

	inline constexpr double kE = 2.7182818284590452353602874713527;

	enum ObjectType {
		kObjectBase,
		kObjectBullet,
		kObjectLaser,
		kObjectBentLaser,
		kObjectEnemy,
		kObjectPlayerShot,
		kObjectParticle,
	};

	enum BulletType {
		kBulletNormal,
		kBulletMedium,
		kBulletScale,
		kBulletLaser,
		kBulletLight,
		kBulletBig,
	};

	enum ParticleType {
		kParticleLight,
		kParticleStar,
		kParticleRect,
	};

	enum EnemyType {
		kEnemyNormal,
	};

	enum PlayerShotType {
		kPlayerShotNormal,
	};

	enum EaseType {
		kLinear,

		kEaseInSine, kEaseOutSine, kEaseInOutSine,
		kEaseInQuad, kEaseOutQuad, kEaseInOutQuad,
		kEaseInCubic, kEaseOutCubic, kEaseInOutCubic,
		kEaseInQuart, kEaseOutQuart, kEaseInOutQuart,
		kEaseInQuint, kEaseOutQuint, kEaseInOutQuint,
		kEaseInExpo, kEaseOutExpo, kEaseInOutExpo,
		kEaseInCirc, kEaseOutCirc, kEaseInOutCirc,
		kEaseInBack, kEaseOutBack, kEaseInOutBack,
		kEaseInElastic, kEaseOutElastic, kEaseInOutElastic,
		kEaseInBounce, kEaseOutBounce, kEaseInOutBounce,
	};

	enum AimType {
		kAimFalse,
		kAimTrue,
		kAimOffset,
	};
}

#endif
