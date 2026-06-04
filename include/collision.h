#pragma once
#ifndef ZENITHSTG_SRC_COLLISION_H_
#define ZENITHSTG_SRC_COLLISION_H_

#include <cmath>

#include "vec2d.h"

namespace zenithstgv {
/**
 * @brief 点と円形の当たり判定 / Point and circular hit detection.
 *
 * @param v1 座標1 / Position1
 * @param v2 座標2 / Position2
 * @param r 半径 / Radius
 */
int ColPointAndCircle(const Vec2D &v1, const Vec2D &v2, double r);

/**
 * @brief 点と三角形の当たり判定 / Point and triangle pertinence.
 *
 * @param v 点の座標 / Point position
 * @param v1 三角形の頂点座標1 / Triangle vertex coordinates1
 * @param v2 三角形の頂点座標2 / Triangle vertex coordinates2
 * @param v3 三角形の頂点座標3 / Triangle vertex coordinates3
 */
int ColPointAndTriangle(const Vec2D &v, const Vec2D &v1, const Vec2D &v2,
                        const Vec2D &v3);

/**
 * @brief 点と矩形の当たり判定 / Point and rectangle pertinence.
 *
 * @param v 点の座標 / Point position
 * @param v1 矩形の頂点座標1 / Rectangle vertex coordinates1
 * @param v2 矩形の頂点座標2 / Rectangle vertex coordinates2
 * @param v3 矩形の頂点座標3 / Rectangle vertex coordinates3
 * @param v4 矩形の頂点座標4 / Rectangle vertex coordinates4
 */
int ColPointAndRect(const Vec2D &v, const Vec2D &v1, const Vec2D &v2,
                    const Vec2D &v3, const Vec2D &v4);

int ColPointAndCircleAdv(const Vec2D &v1, const Vec2D &v2, const Vec2D &v3,
                         double r);
} // namespace zenithstgv

#endif
