#include "object/object.h"

#include <algorithm>
#include <any>
#include <array>
#include <cmath>
#include <string>
#include <vector>

#include "collision.h"
#include "color.h"
#include "easing.h"
#include "vec2d.h"

namespace zenithstgv {
void Object::UpdateObject(long long t, ObjectManager &om, Player &player) {
	if (!(flags_ & kIsAlive))
		return;

	UpdateEase(t);

	vec_ = AngleToVec2D(angle_);
	GrazeObject(om, player);
	MoveFunc(om, player);
}

void Object::DrawObject(const AtlasBuilder &,
                        std::array<std::vector<InstanceData>, 4> &) {}

void Object::UpdateEase(long long t) {
	double elapsed_frame = t - pop_t_;
	if (angle_ease_time_ == 0) {
		angle_ = end_angle_;
	} else {
		angle_t_ = EasingTimeCalc(elapsed_frame, 0, angle_ease_time_);
		angle_ = Easing(angle_ease_type_, angle_t_, start_angle_, end_angle_);
	}
	if (is_aligned_angle_ == 1) {
		show_angle_ = angle_;
	}

	if (speed_ease_time_ == 0) {
		speed_ = end_speed_;
	} else {
		speed_t_ = EasingTimeCalc(elapsed_frame, 0, speed_ease_time_);
		speed_ = Easing(speed_ease_type_, speed_t_, start_speed_, end_speed_);
	}

	if (col_size_ease_time_ == 0) {
		col_size_ = end_col_size_;
	} else {
		col_tize_t_ = EasingTimeCalc(elapsed_frame, 0, col_size_ease_time_);
		col_size_ = Easing(col_size_ease_type_, col_tize_t_, start_col_size_,
		                   end_col_size_);
	}

	if (size_ease_time_ == 0) {
		size_ = end_size_;
	} else {
		size_t_ = EasingTimeCalc(elapsed_frame, 0, size_ease_time_);
		size_ = Easing(size_ease_type_, size_t_, start_size_, end_size_);
	}
}

void Object::MoveObject(double speed) { pos_ += vec_ * speed; }

int Object::ColliCheckObject(ObjectManager &, Player &) { return 0; }

void Object::GrazeObject(ObjectManager &, Player &) {}

int Object::CheckPosBounds(ObjectManager &) { return 0; }

int Object::CheckCollisionAndBounds(ObjectManager &om, Player &player) {
	if (flags_ & kIsCol) {
		if (ColliCheckObject(om, player)) {
			flags_ &= ~kIsAlive;
			return 1;
		}
	}
	if (CheckPosBounds(om)) {
		flags_ &= ~kIsAlive;
		return 1;
	}
	return 0;
}

void Object::MoveFunc(ObjectManager &om, Player &player) {
	switch (id_) {
	case 0:
	default: {
		int needsMultiStep =
		    speed_ >= col_size_ + player.col_size_ && flags_ & kIsCol;
		if (needsMultiStep) {
			int step = static_cast<int>(std::ceil(speed_ / 1.0f));
			for (int i = 0; i < step; i++) {
				MoveObject(speed_ / step);
				if (flags_ & kIsCol) {
					ColliCheckObject(om, player);
				}
				if (CheckPosBounds(om)) {
					flags_ &= ~kIsAlive;
					break;
				}
			}
		} else {
			MoveObject(speed_);
			if (flags_ & kIsCol) {
				ColliCheckObject(om, player);
			}
			if (CheckPosBounds(om))
				flags_ &= ~kIsAlive;
		}
	}
	}
}

void Object::KillObject(ObjectManager &) { flags_ &= ~kIsAlive; }

void ObjectManager::InitManager() {}
void ObjectManager::PushBlankObjects(int) {}
} // namespace zenithstgv
