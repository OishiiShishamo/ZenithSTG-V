#pragma once
#ifndef ZENITHSTGV_INCLUDE_EASING_H_
#define ZENITHSTGV_INCLUDE_EASING_H_

#include <cmath>

namespace zenithstgv {
// ラッパ関数
double Easing(int ease_type, double t, double start, double end);

// イージング時間計算関数
double EasingTimeCalc(double now_time, double start_time, double end_time);

double Linear(double t, double start, double end);

double EaseInSine(double t, double start, double end);
double EaseOutSine(double t, double start, double end);
double EaseInOutSine(double t, double start, double end);

double EaseInQuad(double t, double start, double end);
double EaseOutQuad(double t, double start, double end);
double EaseInOutQuad(double t, double start, double end);

double EaseInCubic(double t, double start, double end);
double EaseOutCubic(double t, double start, double end);
double EaseInOutCubic(double t, double start, double end);

double EaseInQuart(double t, double start, double end);
double EaseOutQuart(double t, double start, double end);
double EaseInOutQuart(double t, double start, double end);

double EaseInQuint(double t, double start, double end);
double EaseOutQuint(double t, double start, double end);
double EaseInOutQuint(double t, double start, double end);

double EaseInExpo(double t, double start, double end);
double EaseOutExpo(double t, double start, double end);
double EaseInOutExpo(double t, double start, double end);

double EaseInCirc(double t, double start, double end);
double EaseOutCirc(double t, double start, double end);
double EaseInOutCirc(double t, double start, double end);

double EaseInBack(double t, double start, double end);
double EaseOutBack(double t, double start, double end);
double EaseInOutBack(double t, double start, double end);

double EaseInElastic(double t, double start, double end);
double EaseOutElastic(double t, double start, double end);
double EaseInOutElastic(double t, double start, double end);

double EaseInBounce(double t, double start, double end);
double EaseOutBounce(double t, double start, double end);
double EaseInOutBounce(double t, double start, double end);
} // namespace zenithstgv

#endif
