#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_TIMEUTL_H_
#define ZENITHSTGV_INCLUDE_UTILIRY_TIMEUTL_H_

#include <chrono>
#include <string>
#include <thread>

#include "main.h"

namespace zenithstgv {
extern long long t;
extern long long fps;

class TimeUtl {
  public:
	static long long target_t_;

	static void InitTimeUtl();

	static void StartTimer();
	static void StopTimer();
	static void ResetTimer();
	static std::chrono::milliseconds NSec2MSec(std::chrono::nanoseconds ns);
	static double NSec2Double(std::chrono::nanoseconds);
	static std::chrono::nanoseconds Timer();
	static void ElapsedTime();
	static void FrameWait();

  private:
	static std::chrono::nanoseconds last_frame_time_;
	static std::chrono::nanoseconds elapsedus_;
	static std::chrono::high_resolution_clock::time_point start_time_;
	static std::chrono::nanoseconds frame_duration_;
};

extern TimeUtl time_mng_;
} // namespace zenithstgv

#endif
