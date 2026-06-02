#pragma once
#ifndef ZENITHSTGV_INCLUDE_UTILITY_TIMEUTL_H_
#define ZENITHSTGV_INCLUDE_UTILIRY_TIMEUTL_H_

#include <chrono>
#include <string>
#include <thread>

#include "main.h"

namespace zenithstgv {
class TimeUtl {
  public:
	long long t = 0;
	long long fps = 60;
	long long target_t_ = 0;

	void StartTimer();
	void StopTimer();
	void RequestStop();
	void ResetTimer();
	std::chrono::milliseconds NSec2MSec(std::chrono::nanoseconds ns);
	double NSec2Double(std::chrono::nanoseconds);
	std::chrono::nanoseconds Timer();
	void ElapsedTime();
	void FrameWait();

  private:
	std::chrono::nanoseconds last_frame_time_ = std::chrono::nanoseconds(0);
	std::chrono::nanoseconds elapsedus_ = std::chrono::nanoseconds(0);
	std::chrono::high_resolution_clock::time_point start_time_;
	std::chrono::nanoseconds frame_duration_ =
	    std::chrono::nanoseconds(1000000000 / fps);
	bool stop_requested_ = false;
};

extern TimeUtl time_mng_;
} // namespace zenithstgv

#endif
