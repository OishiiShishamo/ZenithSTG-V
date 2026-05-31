#include "utility/time_utl.h"

#include "main.h"
#include <chrono>
#include <thread>

namespace zenithstgv {
void TimeUtl::StartTimer() {
	start_time_ = std::chrono::high_resolution_clock::now();
	last_frame_time_ = std::chrono::nanoseconds(0);
	target_t_ = 0;
	t = 0;
}

void TimeUtl::StopTimer() { elapsedus_ = Timer(); }

void TimeUtl::ResetTimer() {
	StartTimer();
	elapsedus_ = std::chrono::nanoseconds(0);
}

std::chrono::milliseconds TimeUtl::NSec2MSec(std::chrono::nanoseconds ns) {
	return duration_cast<std::chrono::milliseconds>(ns);
}

double TimeUtl::NSec2Double(std::chrono::nanoseconds ns) {
	return std::chrono::duration<double>(ns).count();
}

std::chrono::nanoseconds TimeUtl::Timer() {
	return elapsedus_ +
	       std::chrono::duration_cast<std::chrono::nanoseconds>(
	           std::chrono::high_resolution_clock::now() - start_time_);
}

void TimeUtl::ElapsedTime() {
	last_frame_time_ = Timer();
	target_t_ = static_cast<double>(Timer().count()) /
	            static_cast<double>(frame_duration_.count());
}

void TimeUtl::FrameWait() {
	auto now = Timer();
	auto frame_elapsed = now - last_frame_time_;

	auto sleep_margin = std::chrono::milliseconds(4);

	if (frame_elapsed < frame_duration_ - sleep_margin) {
		std::this_thread::sleep_for((frame_duration_ - frame_elapsed) -
		                            sleep_margin);
	}

	// 微調整 / fine tuning.
	while (Timer() - last_frame_time_ < frame_duration_) {
		std::this_thread::yield();
	}
}

TimeUtl time_mng_;
} // namespace zenithstgv
