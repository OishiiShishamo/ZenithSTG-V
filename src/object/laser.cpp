#include "object/laser.h"

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
#include "player.h"
#include "vec2d.h"

namespace zenithstgv {
void Laser::DrawObject(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 4> &instance_lists) {
	if (!(flags_ & kIsAlive))
		return;

	double half_w = width_ / 2;
	std::array<Vec2D, 4> local = {Vec2D(-half_w, 0), Vec2D(half_w, 0),
	                              Vec2D(half_w, length_),
	                              Vec2D(-half_w, length_)};
	std::array<Vec2D, 4> world;
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

int Laser::ColliCheckObject(ObjectManager &, Player &player) {
	double r = (length_ * length_ + col_size_ * col_size_) / 4;
	Vec2D d = pos_ - player.GetPos();
	if (d.GetX() * d.GetX() + d.GetY() * d.GetY() > r) {
		return 0;
	}

	double half_w = col_size_ / 2;
	std::array<Vec2D, 4> local = {Vec2D(-half_w, 0), Vec2D(-half_w, length_),
	                              Vec2D(half_w, length_), Vec2D(half_w, 0)};
	std::array<Vec2D, 4> world;
	for (int i = 0; i < 4; ++i) {
		Vec2D rot = RotatePoint(local[i], show_angle_ + kPi / 2);
		world[i] = pos_ + rot;
	}

	if (ColPointAndRect(player.GetPos(), world[0], world[1], world[2],
	                    world[3])) {
		player.HitPlayer();
		return 1;
	}
	return 0;
}

void Laser::GrazeObject(ObjectManager &, Player &player) {
	if (kGrazeEnabled == 0)
		return;
	if ((flags_ & kIsGraze) == 0)
		return;

	double r = (length_ * length_ + col_size_ * col_size_ +
	            col_size_ * col_size_ + 10 * 10) /
	           4;
	Vec2D d = pos_ - player.GetPos();
	if (d.GetX() * d.GetX() + d.GetY() * d.GetY() > r) {
		return;
	}

	double half_w = (col_size_ + 10) / 2;
	std::array<Vec2D, 4> local = {Vec2D(-half_w, 0), Vec2D(-half_w, length_),
	                              Vec2D(half_w, length_), Vec2D(half_w, 0)};
	std::array<Vec2D, 4> world;
	for (int i = 0; i < 4; ++i) {
		Vec2D rot = RotatePoint(local[i], show_angle_ + kPi / 2);
		world[i] = pos_ + rot;
	}

	if (ColPointAndRect(player.GetPos(), world[0], world[1], world[2],
	                    world[3])) {
		// Graze();
		if (kLaserGrazeEveryFrame == 0) {
			flags_ &= ~kIsGraze;
		}
	}
}

int Laser::CheckPosBounds(ObjectManager &om) {
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

int Laser::CheckCollisionAndBounds(ObjectManager &om, Player &player) {
	if (flags_ & kIsCol) {
		if (ColliCheckObject(om, player)) {
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

void Laser::MoveFunc(ObjectManager &om, Player &player) {
	switch (id_) {
	case 0:
	default: {
		MoveObject(speed_);
		if (CheckCollisionAndBounds(om, player))
			return;
		break;
	}
	}
}

void Laser::KillObject(ObjectManager &om) {
	om.PushBlankObjects(index_);
	flags_ &= ~kIsAlive;
}

void LaserManager::InitManager() {
	default_lasers_blend_["bullet_normal"] = BlendMode::kAdd;
	default_lasers_blend_["bullet_medium"] = BlendMode::kAdd;
	default_lasers_blend_["bullet_big"] = BlendMode::kAdd;
	default_lasers_blend_["bullet_scale"] = BlendMode::kAdd;
	default_lasers_blend_["bullet_light"] = BlendMode::kAdd;

	default_lasers_blend_["laser_normal"] = BlendMode::kAdd;

	lasers_graph_size_["bullet_normal"] = 20;
	lasers_graph_size_["bullet_medium"] = 40;
	lasers_graph_size_["bullet_big"] = 96;
	lasers_graph_size_["bullet_scale"] = 20;
	lasers_graph_size_["bullet_light"] = 30;

	lasers_graph_size_["laser_normal"] = 8;

	for (uint32_t i = 0; i < kMaxLaser; i++) {
		laser_ptrs_[i] = &lasers_[i];
		blank_lasers_.emplace_back(i);
	}
}

void LaserManager::PushBlankObjects(int idx) {
	std::lock_guard<std::mutex> lock(blank_lasers_mutex_);
	blank_lasers_.emplace_back(idx);
}

int LaserManager::CreateLaser(
    long long t, const Player &player, const Vec2D &pos, double length,
    double width, const Color &color, std::string style, BlendMode blend,
    float pal, int is_col, double start_col_size, double end_col_size,
    int col_size_ease_type, int col_size_ease_time, double start_size,
    double end_size, int size_ease_type, int size_ease_time, int aim,
    double start_angle, double end_angle, int angle_ease_type,
    int angle_ease_time, double start_speed, double end_speed,
    int speed_ease_type, int speed_ease_time, int id, int priority,
    const std::vector<std::any> &params) {
	if (blank_lasers_.empty())
		return 1;
	int idx = blank_lasers_.back();
	blank_lasers_.pop_back();

	lasers_[idx].flags_ = kIsAlive | is_col * kIsCol | kIsGraze;
	lasers_[idx].obj_type_ = kObjectLaser;
	lasers_[idx].pos_ = pos;
	lasers_[idx].color_ = color;
	lasers_[idx].style_ = style;
	lasers_[idx].blend_ = blend;
	lasers_[idx].pal_ = pal;
	lasers_[idx].start_col_size_ = start_col_size;
	lasers_[idx].end_col_size_ = end_col_size;
	lasers_[idx].col_size_ease_type_ = col_size_ease_type;
	lasers_[idx].col_size_ease_time_ = col_size_ease_time;
	lasers_[idx].start_size_ = start_size;
	lasers_[idx].end_size_ = end_size;
	lasers_[idx].size_ease_type_ = size_ease_type;
	lasers_[idx].size_ease_time_ = size_ease_time;
	if (aim == kAimTrue) {
		lasers_[idx].start_angle_ = player.AimPlayer(pos) + start_angle;
		lasers_[idx].end_angle_ = player.AimPlayer(pos) + end_angle;
	} else {
		lasers_[idx].start_angle_ = start_angle;
		lasers_[idx].end_angle_ = end_angle;
	}
	lasers_[idx].angle_ease_type_ = angle_ease_type;
	lasers_[idx].angle_ease_time_ = angle_ease_time;
	lasers_[idx].start_speed_ = start_speed;
	lasers_[idx].end_speed_ = end_speed;
	lasers_[idx].speed_ease_type_ = speed_ease_type;
	lasers_[idx].speed_ease_time_ = speed_ease_time;
	lasers_[idx].pop_t_ = t;
	lasers_[idx].length_ = length;
	lasers_[idx].width_ = width;
	lasers_[idx].front_node_ = 0;
	lasers_[idx].current_node_num_ = 0;
	lasers_[idx].order_ = laser_index_++;
	lasers_[idx].index_ = idx;
	lasers_[idx].id_ = id;
	lasers_[idx].priority_ = priority;
	lasers_[idx].params_ = params;
	return 0;
}

void LaserManager::CreateLaserGroup(
    long long t, const Player &player, const Vec2D &pos, double length,
    double width, const Color &color, std::string style, BlendMode blend,
    float pal, int is_col, double start_col_size, double end_col_size,
    int col_size_ease_type, int col_size_ease_time, double start_size,
    double end_size, int size_ease_type, int size_ease_time, int way,
    double spread, int aim, double start_angle, double end_angle,
    int angle_ease_type, int angle_ease_time, double start_speed,
    double end_speed, int speed_ease_type, int speed_ease_time, int id,
    int priority, const std::vector<std::any> &params) {
	switch (aim) {
	case kAimFalse:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, is_col, start_col_size, end_col_size,
			                col_size_ease_type, col_size_ease_time, start_size,
			                end_size, size_ease_type, size_ease_time, 0,
			                spread / way * i + start_angle - spread / 2,
			                spread / way * i + end_angle - spread / 2,
			                angle_ease_type, angle_ease_time, start_speed,
			                end_speed, speed_ease_type, speed_ease_time, id,
			                priority, params))
				return;
		}
		break;
	case kAimTrue:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, is_col, start_col_size, end_col_size,
			                col_size_ease_type, col_size_ease_time, start_size,
			                end_size, size_ease_type, size_ease_time, 1,
			                spread / way * i + start_angle - spread / 2,
			                spread / way * i + end_angle - spread / 2,
			                angle_ease_type, angle_ease_time, start_speed,
			                end_speed, speed_ease_type, speed_ease_time, id,
			                priority, params))
				return;
		}
		break;
	case kAimOffset:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, is_col, start_col_size, end_col_size,
			                col_size_ease_type, col_size_ease_time, start_size,
			                end_size, size_ease_type, size_ease_time, 1,
			                spread / way * i + start_angle +
			                    spread / (way * 2) - spread / 2,
			                spread / way * i + end_angle + spread / (way * 2) -
			                    spread / 2,
			                angle_ease_type, angle_ease_time, start_speed,
			                end_speed, speed_ease_type, speed_ease_time, id,
			                priority, params))
				return;
		}
		break;
	default:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, is_col, start_col_size, end_col_size,
			                col_size_ease_type, col_size_ease_time, start_size,
			                end_size, size_ease_type, size_ease_time, 0,
			                spread / way * i + start_angle - spread / 2,
			                spread / way * i + end_angle - spread / 2,
			                angle_ease_type, angle_ease_time, start_speed,
			                end_speed, speed_ease_type, speed_ease_time, id,
			                priority, params))
				return;
		}
		break;
	}
}

void LaserManager::CreateSimpleLaserGroup(
    long long t, const Player &player, const Vec2D &pos, double length,
    double width, const Color &color, std::string style, BlendMode blend,
    float pal, double col_size, double size, int way, double spread, int aim,
    double angle, double speed, int id, const std::vector<std::any> &params) {
	switch (aim) {
	case kAimFalse:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, 1, col_size, col_size, 0, 0, size, size, 0, 0,
			                0, spread / way * i + angle - spread / 2,
			                spread / way * i + angle - spread / 2, 0, 0, speed,
			                speed, 0, 0, id, 0, params))
				return;
		}
		break;
	case kAimTrue:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, 1, col_size, col_size, 0, 0, size, size, 0, 0,
			                1, spread / way * i + angle - spread / 2,
			                spread / way * i + angle - spread / 2, 0, 0, speed,
			                speed, 0, 0, id, 0, params))
				return;
		}
		break;
	case kAimOffset:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(
			        t, player, pos, length, width, color, style, blend, pal, 1,
			        col_size, col_size, 0, 0, size, size, 0, 0, 1,
			        spread / way * i + angle + spread / (way * 2) - spread / 2,
			        spread / way * i + angle + spread / (way * 2) - spread / 2,
			        0, 0, speed, speed, 0, 0, id, 0, params))
				return;
		}
		break;
	default:
		for (int i = 0; i < way; i++) {
			if (CreateLaser(t, player, pos, length, width, color, style, blend,
			                pal, 1, col_size, col_size, 0, 0, size, size, 0, 0,
			                0, spread / way * i + angle - spread / 2,
			                spread / way * i + angle - spread / 2, 0, 0, speed,
			                speed, 0, 0, id, 0, params))
				return;
		}
		break;
	}
}

void LaserManager::CreateSmartLaserGroup(long long t, const Player &player,
                                         ObjectParams param) {
	switch (param.aim) {
	case kAimFalse:
		for (int i = 0; i < param.way; i++) {
			if (CreateLaser(t, player, param.pos, param.length, param.width,
			                param.color, param.style, param.blend, param.pal,
			                param.is_col, param.start_col_size,
			                param.end_col_size, param.col_size_ease_type,
			                param.col_size_ease_time, param.start_size,
			                param.end_size, param.size_ease_type,
			                param.size_ease_time, 0,
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
			if (CreateLaser(t, player, param.pos, param.length, param.width,
			                param.color, param.style, param.blend, param.pal,
			                param.is_col, param.start_col_size,
			                param.end_col_size, param.col_size_ease_type,
			                param.col_size_ease_time, param.start_size,
			                param.end_size, param.size_ease_type,
			                param.size_ease_time, 1,
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
			if (CreateLaser(
			        t, player, param.pos, param.length, param.width,
			        param.color, param.style, param.blend, param.pal,
			        param.is_col, param.start_col_size, param.end_col_size,
			        param.col_size_ease_type, param.col_size_ease_time,
			        param.start_size, param.end_size, param.size_ease_type,
			        param.size_ease_time, 1,
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
			if (CreateLaser(t, player, param.pos, param.length, param.width,
			                param.color, param.style, param.blend, param.pal,
			                param.is_col, param.start_col_size,
			                param.end_col_size, param.col_size_ease_type,
			                param.col_size_ease_time, param.start_size,
			                param.end_size, param.size_ease_type,
			                param.size_ease_time, 0,
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

void LaserManager::ParallelUpdateLasers(long long t, Player &player,
                                        std::array<Laser, kMaxLaser> &lasers) {
	std::for_each(
	    std::execution::par, lasers.begin(), lasers.end(),
	    [this, t, &player](Laser &L) { L.UpdateObject(t, *this, player); });
}

void LaserManager::MoveLasers(long long t, Player &player) {
	ParallelUpdateLasers(t, player, lasers_);
}

void LaserManager::RenderLasers(
    const AtlasBuilder &atlas,
    std::array<std::vector<InstanceData>, 4> &instance_lists) {
	std::sort(laser_ptrs_.begin(), laser_ptrs_.end(),
	          [](const Laser *a, const Laser *b) {
		          if (a->priority_ == b->priority_) {
			          return a->order_ < b->order_;
		          }
		          return a->priority_ < b->priority_;
	          });
	for (auto *L : laser_ptrs_) {
		L->DrawObject(atlas, instance_lists);
	}
}

BlendMode LaserManager::GetDefaultObjectBlend(std::string style) {
	return default_lasers_blend_.at(style);
}

int LaserManager::GetObjectGraphSize(std::string style) {
	return lasers_graph_size_.at(style);
}

double LaserManager::GetObjectColSize(std::string style) {
	(void)style;
	return 0.0;
}

double LaserManager::GetObjectGrazeSize(std::string style) {
	(void)style;
	return 0.0;
}
} // namespace zenithstgv
