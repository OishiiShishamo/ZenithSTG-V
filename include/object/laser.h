/**
 * @file laser.h
 * @brief レーザー関連 / Laser Related.
 * @auther ZenithSTG Dev Team
 */

#pragma once
#ifndef ZENITHSTGV_INCLUDE_OBJECT_LASER_H_
#define ZENITHSTGV_INCLUDE_OBJECT_LASER_H_

#include <array>
#include <cmath>
#include <execution>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "collision.h"
#include "color.h"
#include "global.h"
#include "instance_data.h"
#include "object/object.h"
#include "player.h"
#include "utility/atlas_builder.h"
#include "vec2d.h"

namespace zenithstgv {

class LaserManager;

/**
 * @class Laser
 * @brief レーザー関連 / Laser Related.
 * @extends Object
 */
class Laser : public Object {
  public:
	Laser() = default;
	Laser(int alive, int is_col, const Vec2D &pos, double start_angle,
	      double end_angle, int angle_ease_type, int angle_ease_time,
	      double start_show_angle, double end_show_angle,
	      int show_angle_ease_type, int show_angle_ease_time,
	      const Color &color, std::string style, BlendMode blend, float pal,
	      double start_col_size, double end_col_size, int col_size_ease_type,
	      int col_size_ease_time, double start_size, double end_size,
	      int size_ease_type, int size_ease_time, double start_speed,
	      double end_speed, int speed_ease_type, int speed_ease_time, int pop_t,
	      double length, double width, int id = 0, int priority = 0,
	      const std::vector<std::any> &params = {})
	    : Object(alive, is_col, kObjectLaser, pos, start_angle, end_angle,
	             angle_ease_type, angle_ease_time, start_show_angle,
	             end_show_angle, show_angle_ease_type, show_angle_ease_time,
	             color, style, blend, pal, start_col_size, end_col_size,
	             col_size_ease_type, col_size_ease_time, start_size, end_size,
	             size_ease_type, size_ease_time, start_speed, end_speed,
	             speed_ease_type, speed_ease_time, pop_t, length, width, 0, 0,
	             0, 0, 0, id, priority, params) {}

	/**
	 * @brief レーザーの描画 / Laser Drawing.
	 */
	void DrawObject(const AtlasBuilder &atlas,
	                std::array<std::vector<InstanceData>, 5> &instance_lists,
	                ObjectManager &om) override;

  private:
	void MoveFunc(ObjectManager &om, Player &player) override;
	void KillObject(ObjectManager &om) override;
	int ColliCheckObject(ObjectManager &om, Player &player) override;
	void GrazeObject(ObjectManager &om, Player &player) override;
	int CheckPosBounds(ObjectManager &om) override;
	int CheckCollisionAndBounds(ObjectManager &om, Player &player) override;
};

class LaserManager : public ObjectManager {
  public:
	void InitManager() override;

	/**
	 * @brief レーザーの生成 / Create Laser.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	int CreateLaser(long long t, const Player &player, const Vec2D &pos,
	                double length, double width, const Color &color,
	                std::string style, BlendMode blend, float pal, int is_col,
	                double start_col_size, double end_col_size,
	                int col_size_ease_type, int col_size_ease_time,
	                double start_size, double end_size, int size_ease_type,
	                int size_ease_time, int aim, double start_angle,
	                double end_angle, int angle_ease_type, int angle_ease_time,
	                double start_speed, double end_speed, int speed_ease_type,
	                int speed_ease_time, int id = 0, int priority = 0,
	                const std::vector<std::any> &params = {});

	/**
	 * @brief 複数のレーザーの生成 / Create Multiple Laser.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	void CreateLaserGroup(
	    long long t, const Player &player, const Vec2D &pos, double length,
	    double width, const Color &color, std::string style, BlendMode blend,
	    float pal, int is_col, double start_col_size, double end_col_size,
	    int col_size_ease_type, int col_size_ease_time, double start_size,
	    double end_size, int size_ease_type, int size_ease_time, int way,
	    double spread, int aim, double start_angle, double end_angle,
	    int angle_ease_type, int angle_ease_time, double start_speed,
	    double end_speed, int speed_ease_type, int speed_ease_time, int id = 0,
	    int priority = 0, const std::vector<std::any> &params = {});

	/**
	 * @brief 複数のレーザーの生成(引数減少版) / Create Multiple Laser (reduced
	 * argument version).
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	void CreateSimpleLaserGroup(long long t, const Player &player,
	                            const Vec2D &pos, double length, double width,
	                            const Color &color, std::string style,
	                            BlendMode blend, float pal, double col_size,
	                            double size, int way, double spread, int aim,
	                            double angle, double speed, int id = 0,
	                            const std::vector<std::any> &params = {});

	/**
	 * @brief 構造体を引数とした複数のレーザーの生成 / Create Multiple Laser
	 * with structure as argument.
	 *
	 * @see ObjectParams 引数の詳細はこちらを参照 / See here for details of the
	 * argument.
	 */
	void CreateSmartLaserGroup(long long t, const Player &player,
	                           ObjectParams param);

	void PushBlankObjects(int idx) override;

	/**
	 * @brief レーザーの動作 / Laser Movement.
	 */
	void MoveLasers(long long t, Player &player);

	/**
	 * @brief レーザーの描画 / Laser Drawing.
	 */
	void RenderLasers(const AtlasBuilder &atlas,
	                  std::array<std::vector<InstanceData>, 5> &instance_lists);

	BlendMode GetDefaultObjectBlend(std::string style) override;
	int GetObjectGraphSize(std::string style) override;
	double GetObjectColSize(std::string style) override;
	double GetObjectGrazeSize(std::string style) override;

  private:
	std::array<Laser, kMaxLaser> lasers_;
	std::array<Laser *, kMaxLaser> laser_ptrs_;
	std::vector<int> blank_lasers_;
	std::mutex blank_lasers_mutex_;
	std::unordered_map<std::string, BlendMode> default_lasers_blend_;
	std::unordered_map<std::string, int> lasers_graph_size_;
	long long laser_index_ = 0;

	void ParallelUpdateLasers(long long t, Player &player,
	                          std::array<Laser, kMaxLaser> &lasers);
};

} // namespace zenithstgv

#endif
