/**
 * @file Bullet.h
 * @brief 弾関連 / Bullet Related.
 * @auther ZenithSTG Dev Team
 */

#pragma once
#ifndef ZENITHSTGV_INCLUDE_OBJECT_BULLET_H_
#define ZENITHSTGV_INCLUDE_OBJECT_BULLET_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <execution>
#include <mutex>
#include <string>
#include <vector>

#include "collision.h"
#include "color.h"
#include "global.h"
#include "instance_data.h"
#include "object/object.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {
class BulletManager;

/**
 * @class Bullet
 * @brief 弾関連 / Bullet Related.
 * @extends Object
 */
class Bullet : public Object {
  public:
	Bullet() = default;
	Bullet(int alive, int is_col, const Vec2D &pos, double start_angle,
	       double end_angle, int angle_ease_type, int angle_ease_time,
	       double start_show_angle, double end_show_angle,
	       int show_angle_ease_type, int show_angle_ease_time,
	       const Color &color, std::string style, BlendMode blend, float pal,
	       double start_col_size, double end_col_size, int col_size_ease_type,
	       int col_size_ease_time, double start_size, double end_size,
	       int size_ease_type, int size_ease_time, double start_speed,
	       double end_speed, int speed_ease_type, int speed_ease_time,
	       int pop_t, int id = 0, int priority = 0,
	       const std::vector<std::any> &params = {})
	    : Object(alive, is_col, kObjectBullet, pos, start_angle, end_angle,
	             angle_ease_type, angle_ease_time, start_show_angle,
	             end_show_angle, show_angle_ease_type, show_angle_ease_time,
	             color, style, blend, pal, start_col_size, end_col_size,
	             col_size_ease_type, col_size_ease_time, start_size, end_size,
	             size_ease_type, size_ease_time, start_speed, end_speed,
	             speed_ease_type, speed_ease_time, pop_t, 0, 0, 0, 0, 0, 0, 0,
	             id, priority, params) {}

	/**
	 * @brief 弾の描画 / Bullet Drawing.
	 */
	void DrawObject(
	    const AtlasBuilder &atlas,
	    std::array<std::vector<InstanceData>, 4> &instance_lists) override;

  private:
	void MoveFunc(ObjectManager &om) override;
	void KillObject(ObjectManager &om) override;
	int ColliCheckObject(ObjectManager &om) override;
	void GrazeObject(ObjectManager &om) override;
	int CheckPosBounds(ObjectManager &om) override;

	int CheckCollisionAndBounds(ObjectManager &om) override;
};

class BulletManager : public ObjectManager {
  public:
	void InitManager() override;

	/**
	 * @brief 弾の生成 / Create Bullet.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	int CreateBullet(long long t, const Vec2D &pos, const Color &color,
	                 std::string style, BlendMode blend, float pal, int is_col,
	                 double start_col_size, double end_col_size,
	                 int col_size_ease_type, int col_size_ease_time,
	                 double start_size, double end_size, int size_ease_type,
	                 int size_ease_time, int aim, double start_angle,
	                 double end_angle, int angle_ease_type, int angle_ease_time,
	                 double start_speed, double end_speed, int speed_ease_type,
	                 int speed_ease_time, int id, int priority,
	                 const std::vector<std::any> &params);

	/**
	 * @brief 複数の弾の生成 / Create Multiple Bullet.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	void CreateBulletGroup(
	    long long t, const Vec2D &pos, const Color &color, std::string style,
	    BlendMode blend, float pal, int is_col, double start_col_size,
	    double end_col_size, int col_size_ease_type, int col_size_ease_time,
	    double start_size, double end_size, int size_ease_type,
	    int size_ease_time, int way, double spread, int aim, double start_angle,
	    double end_angle, int angle_ease_type, int angle_ease_time,
	    double start_speed, double end_speed, int speed_ease_type,
	    int speed_ease_time, int id = 0, int priority = 0,
	    const std::vector<std::any> &params = {});

	/**
	 * @brief 構造体を引数とした複数の弾の生成 / Create Multiple Bullet with
	 * structure as argument.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	void CreateSmartBulletGroup(long long t, ObjectParams param);

	void PushBlankObjects(int idx) override;

	/**
	 * @brief 弾の動作と描画 / Bullet Movement and Drawing.
	 */
	void MoveBullets(long long t);

	void
	RenderBullets(const AtlasBuilder &atlas,
	              std::array<std::vector<InstanceData>, 4> &instance_lists);

	BlendMode GetDefaultObjectBlend(std::string style) override;

	int GetObjectGraphSize(std::string style) override;

	double GetObjectColSize(std::string style) override;

	double GetObjectGrazeSize(std::string style) override;

  private:
	std::array<Bullet, kMaxBullet> bullets;
	std::array<Bullet *, kMaxBullet> bullet_ptrs;
	std::vector<int> blank_bullets;
	std::mutex blank_bullets_mutex;
	std::unordered_map<std::string, BlendMode> default_bullets_blend;
	std::unordered_map<std::string, int> bullets_graph_size;
	std::unordered_map<std::string, double> bullets_col_size;
	std::unordered_map<std::string, double> bullets_graze_size;
	long long bullet_index = 0;

	void ParallelUpdateBullets(long long t,
	                           std::array<Bullet, kMaxBullet> &bullets);
};
} // namespace zenithstgv

#endif
