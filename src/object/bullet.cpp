#include "object/bullet.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <execution>
#include <mutex>
#include <unordered_map>
#include <vector>

#include "collision.h"
#include "color.h"
#include "global.h"
#include "instance_data.h"
#include "main.h"
#include "object/object.h"
#include "vec2d.h"

namespace zenithstgv {

void Bullet::DrawObject(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 4> &instance_lists) {
	if (!(flags_ & kIsAlive))
		return;
	std::array<Vec2D, 4> world;
	double half = size_ / 2 * 20;
	std::array<Vec2D, 4> local = {Vec2D(-half, -half), Vec2D(-half, half),
	                              Vec2D(half, half), Vec2D(half, -half)};
	for (int i = 0; i < 4; ++i) {
		Vec2D rot = RotatePoint(local[i], show_angle_ + kPi / 2);
		world[i] = pos_ + rot;
	}
	Sprite spr;
	spr.pos[0] = world[0].ToNDC(ScreenSize(1920, 1080));
	spr.pos[1] = world[1].ToNDC(ScreenSize(1920, 1080));
	spr.pos[2] = world[2].ToNDC(ScreenSize(1920, 1080));
	spr.pos[3] = world[3].ToNDC(ScreenSize(1920, 1080));
	spr.color = color_;
	spr.blend_mode = blend_;
	spr.blend_pal = pal_;
	spr.uv_key = style_;
	instance_lists[static_cast<int>(blend_)].push_back(
	    spr.toInstanceData(atlas.getEntries().at(spr.uv_key)));
}

int Bullet::ColliCheckObject(ObjectManager &om) {
	// if (ColPointAndCircle(pos_, player_.GetPos(),
	//                       om.GetObjectColSize(style_) * col_size_ +
	//                       player_.col_size_)) {
	//	player_.HitPlayer();
	//	return 1;
	// }
	return 0;
}

void Bullet::GrazeObject(ObjectManager &om) {
	if (kGrazeEnabled == 0)
		return;
	if ((flags_ & kIsGraze) == 0)
		return;
	// if (ColPointAndCircle(pos_, player_.GetPos(),
	//                       om.GetObjectGrazeSize(style_) * col_size_ +
	//                       player_.col_size_ + kGrazeRange)) {
	// Graze();
	if (kBulletGrazeEveryFrame == 0) {
		flags_ &= ~kIsGraze;
	}
}

int Bullet::CheckPosBounds(ObjectManager &om) {
	double limit = size_ * om.GetObjectGraphSize(style_) * 2;
	if (pos_.GetX() < kBorderLeft - limit)
		return 1;
	if (pos_.GetX() > kBorderRight + limit)
		return 1;
	if (pos_.GetY() < kBorderUp - limit)
		return 1;
	if (pos_.GetY() > kBorderDown + limit)
		return 1;

	return 0;
}

int Bullet::CheckCollisionAndBounds(ObjectManager &om) {
	if (flags_ & kIsCol) {
		if (ColliCheckObject(om)) {
			KillObject(om);
			return 1;
		}
	}
	if (CheckPosBounds(om)) {
		KillObject(om);
		return 1;
	}
	return 0;
}

void Bullet::MoveFunc(ObjectManager &om) {
	switch (id_) {
	case 0:
	default: {
		if (speed_ >= col_size_) { // + player_.col_size_ && flags_ & kIsCol) {
			Vec2D old_pos = pos_;
			MoveObject(speed_);
			// if (ColPointAndCircleAdv(player_.GetPos(), old_pos, pos_,
			//                          col_size_)) {
			//  player_.HitPlayer();
			//	KillObject();
			//	return;
			//}
			if (CheckPosBounds(om)) {
				KillObject(om);
				return;
			}
		} else {
			MoveObject(speed_);
			if (CheckCollisionAndBounds(om))
				return;
		}
		break;
	}
	}
}

void Bullet::KillObject(ObjectManager &om) {
	om.PushBlankObjects(index_);
	flags_ &= ~kIsAlive;
}

void BulletManager::InitManager() {
	default_bullets_blend["bullet_1"] = BlendMode::kNormal;

	bullets_graph_size["bullet_1"] = 20;

	bullets_col_size["bullet_1"] = 18;

	bullets_graze_size["bullet_1"] = 30;

	for (uint32_t i = 0; i < kMaxBullet; i++) {
		bullet_ptrs[i] = &bullets[i];
		blank_bullets.emplace_back(i);
	}
}

void BulletManager::PushBlankObjects(int idx) {
	std::lock_guard<std::mutex> lock(blank_bullets_mutex);
	blank_bullets.emplace_back(idx);
}

int BulletManager::CreateBullet(
    long long t, const Vec2D &pos, const Color &color, std::string style,
    BlendMode blend, float pal, int is_col, double start_col_size,
    double end_col_size, int col_size_ease_type, int col_size_ease_time,
    double start_size, double end_size, int size_ease_type, int size_ease_time,
    int aim, double start_angle, double end_angle, int angle_ease_type,
    int angle_ease_time, double start_speed, double end_speed,
    int speed_ease_type, int speed_ease_time, int id, int priority,
    const std::vector<std::any> &params) {
	// sound_mng_.ReserveSe(se);
	if (blank_bullets.empty())
		return 1;
	int idx = blank_bullets.back();
	blank_bullets.pop_back();
	bullets[idx].flags_ = kIsAlive | is_col * kIsCol | kIsGraze;
	bullets[idx].obj_type_ = kObjectBullet;
	bullets[idx].pos_ = pos;
	bullets[idx].color_ = color;
	bullets[idx].style_ = style;
	bullets[idx].blend_ = blend;
	bullets[idx].pal_ = pal;
	bullets[idx].start_col_size_ = start_col_size;
	bullets[idx].end_col_size_ = end_col_size;
	bullets[idx].col_size_ease_type_ = col_size_ease_type;
	bullets[idx].col_size_ease_time_ = col_size_ease_time;
	bullets[idx].start_size_ = start_size;
	bullets[idx].end_size_ = end_size;
	bullets[idx].size_ease_type_ = size_ease_type;
	bullets[idx].size_ease_time_ = size_ease_time;
	// if (aim == kAimTrue) {
	//	bullets[idx].start_angle_ = player_.AimPlayer(pos) + start_angle;
	//	bullets[idx].end_angle_ = player_.AimPlayer(pos) + end_angle;
	// } else {
	bullets[idx].start_angle_ = start_angle;
	bullets[idx].end_angle_ = end_angle;
	//}
	bullets[idx].angle_ease_type_ = angle_ease_type;
	bullets[idx].angle_ease_time_ = angle_ease_time;
	bullets[idx].start_speed_ = start_speed;
	bullets[idx].end_speed_ = end_speed;
	bullets[idx].speed_ease_type_ = speed_ease_type;
	bullets[idx].speed_ease_time_ = speed_ease_time;
	bullets[idx].pop_t_ = t;
	bullets[idx].length_ = 0;
	bullets[idx].width_ = 0;
	bullets[idx].front_node_ = 0;
	bullets[idx].current_node_num_ = 0;
	bullets[idx].order_ = bullet_index++;
	bullets[idx].index_ = idx;
	bullets[idx].id_ = id;
	bullets[idx].priority_ = priority;
	bullets[idx].params_ = params;
	return 0;
}

void BulletManager::CreateBulletGroup(
    long long t, const Vec2D &pos, const Color &color, std::string style,
    BlendMode blend, float pal, int is_col, double start_col_size,
    double end_col_size, int col_size_ease_type, int col_size_ease_time,
    double start_size, double end_size, int size_ease_type, int size_ease_time,
    int way, double spread, int aim, double start_angle, double end_angle,
    int angle_ease_type, int angle_ease_time, double start_speed,
    double end_speed, int speed_ease_type, int speed_ease_time, int id,
    int priority, const std::vector<std::any> &params) {
	// sound_mng_.ReserveSe(se);
	switch (aim) {
	case kAimFalse:
		for (int i = 0; i < way; i++) {
			if (CreateBullet(
			        t, pos, color, style, blend, pal, is_col, start_col_size,
			        end_col_size, col_size_ease_type, col_size_ease_time,
			        start_size, end_size, size_ease_type, size_ease_time, 0,
			        spread / way * i + start_angle - spread / 2,
			        spread / way * i + end_angle - spread / 2, angle_ease_type,
			        angle_ease_time, start_speed, end_speed, speed_ease_type,
			        speed_ease_time, id, priority, params))
				return;
		}
		break;
	case kAimTrue:
		for (int i = 0; i < way; i++) {
			if (CreateBullet(
			        t, pos, color, style, blend, pal, is_col, start_col_size,
			        end_col_size, col_size_ease_type, col_size_ease_time,
			        start_size, end_size, size_ease_type, size_ease_time, 1,
			        spread / way * i + start_angle - spread / 2,
			        spread / way * i + end_angle - spread / 2, angle_ease_type,
			        angle_ease_time, start_speed, end_speed, speed_ease_type,
			        speed_ease_time, id, priority, params))
				return;
		}
		break;
	case kAimOffset:
		for (int i = 0; i < way; i++) {
			if (CreateBullet(
			        t, pos, color, style, blend, pal, is_col, start_col_size,
			        end_col_size, col_size_ease_type, col_size_ease_time,
			        start_size, end_size, size_ease_type, size_ease_time, 1,
			        spread / way * i + start_angle + spread / (way * 2) -
			            spread / 2,
			        spread / way * i + end_angle + spread / (way * 2) -
			            spread / 2,
			        angle_ease_type, angle_ease_time, start_speed, end_speed,
			        speed_ease_type, speed_ease_time, id, priority, params))
				return;
		}
		break;
	default:
		for (int i = 0; i < way; i++) {
			if (CreateBullet(
			        t, pos, color, style, blend, pal, is_col, start_col_size,
			        end_col_size, col_size_ease_type, col_size_ease_time,
			        start_size, end_size, size_ease_type, size_ease_time, 0,
			        spread / way * i + start_angle - spread / 2,
			        spread / way * i + end_angle - spread / 2, angle_ease_type,
			        angle_ease_time, start_speed, end_speed, speed_ease_type,
			        speed_ease_time, id, priority, params))
				return;
		}
		break;
	}
}

void BulletManager::CreateSmartBulletGroup(long long t, ObjectParams param) {
	// sound_mng_.ReserveSe(param.se);
	switch (param.aim) {
	case kAimFalse:
		for (int i = 0; i < param.way; i++) {
			if (CreateBullet(t, param.pos, param.color, param.style,
			                 param.blend, param.pal, param.is_col,
			                 param.start_col_size, param.end_col_size,
			                 param.col_size_ease_type, param.col_size_ease_time,
			                 param.start_size, param.end_size,
			                 param.size_ease_type, param.size_ease_time, 0,
			                 param.spread / param.way * i + param.start_angle -
			                     param.spread / 2,
			                 param.spread / param.way * i + param.end_angle -
			                     param.spread / 2,
			                 param.angle_ease_type, param.angle_ease_time,
			                 param.start_speed, param.end_speed,
			                 param.speed_ease_type, param.speed_ease_time,
			                 param.id, param.priority, param.params))
				return;
		}
		break;
	case kAimTrue:
		for (int i = 0; i < param.way; i++) {
			if (CreateBullet(t, param.pos, param.color, param.style,
			                 param.blend, param.pal, param.is_col,
			                 param.start_col_size, param.end_col_size,
			                 param.col_size_ease_type, param.col_size_ease_time,
			                 param.start_size, param.end_size,
			                 param.size_ease_type, param.size_ease_time, 1,
			                 param.spread / param.way * i + param.start_angle -
			                     param.spread / 2,
			                 param.spread / param.way * i + param.end_angle -
			                     param.spread / 2,
			                 param.angle_ease_type, param.angle_ease_time,
			                 param.start_speed, param.end_speed,
			                 param.speed_ease_type, param.speed_ease_time,
			                 param.id, param.priority, param.params))
				return;
		}
		break;
	case kAimOffset:
		for (int i = 0; i < param.way; i++) {
			if (CreateBullet(
			        t, param.pos, param.color, param.style, param.blend,
			        param.pal, param.is_col, param.start_col_size,
			        param.end_col_size, param.col_size_ease_type,
			        param.col_size_ease_time, param.start_size, param.end_size,
			        param.size_ease_type, param.size_ease_time, 1,
			        param.spread / param.way * i + param.start_angle +
			            param.spread / (param.way * 2) - param.spread / 2,
			        param.spread / param.way * i + param.end_angle +
			            param.spread / (param.way * 2) - param.spread / 2,
			        param.angle_ease_type, param.angle_ease_time,
			        param.start_speed, param.end_speed, param.speed_ease_type,
			        param.speed_ease_time, param.id, param.priority,
			        param.params))
				return;
		}
		break;
	default:
		for (int i = 0; i < param.way; i++) {
			if (CreateBullet(t, param.pos, param.color, param.style,
			                 param.blend, param.pal, param.is_col,
			                 param.start_col_size, param.end_col_size,
			                 param.col_size_ease_type, param.col_size_ease_time,
			                 param.start_size, param.end_size,
			                 param.size_ease_type, param.size_ease_time, 0,
			                 param.spread / param.way * i + param.start_angle -
			                     param.spread / 2,
			                 param.spread / param.way * i + param.end_angle -
			                     param.spread / 2,
			                 param.angle_ease_type, param.angle_ease_time,
			                 param.start_speed, param.end_speed,
			                 param.speed_ease_type, param.speed_ease_time,
			                 param.id, param.priority, param.params))
				return;
		}
		break;
	}
}

void BulletManager::ParallelUpdateBullets(
    long long t, std::array<Bullet, kMaxBullet> &bullets) {
	std::for_each(std::execution::par, bullets.begin(), bullets.end(),
	              [this, t](Bullet &B) { B.UpdateObject(t, *this); });
}

void BulletManager::MoveBullets(long long t) {
	ParallelUpdateBullets(t, bullets);
}

void BulletManager::RenderBullets(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 4> &instance_lists) {
	std::sort(bullet_ptrs.begin(), bullet_ptrs.end(),
	          [](const Bullet *a, const Bullet *b) {
		          if (a->priority_ == b->priority_) {
			          return a->order_ < b->order_;
		          }
		          return a->priority_ < b->priority_;
	          });
	for (auto *B : bullet_ptrs) {
		B->DrawObject(atlas, instance_lists);
	}
}

BlendMode BulletManager::GetDefaultObjectBlend(std::string style) {
	return default_bullets_blend.at(style);
}

int BulletManager::GetObjectGraphSize(std::string style) {
	return bullets_graph_size.at(style);
}

double BulletManager::GetObjectColSize(std::string style) {
	return bullets_col_size.at(style);
}

double BulletManager::GetObjectGrazeSize(std::string style) {
	return bullets_graze_size.at(style);
}
} // namespace zenithstgv
