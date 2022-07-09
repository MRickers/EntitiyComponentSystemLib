#include <utils/clock_chrono.h>

namespace utils::clock {
	Chrono::Chrono() : start_time_(std::chrono::high_resolution_clock::now()), stop_time_(start_time_) {}

	utils::time_ms Chrono::TimePassed() const {
		const auto duration = stop_time_ - start_time_;
		const auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
		return duration_ms.count();
	}

	void Chrono::Start() {
		start_time_ = std::chrono::high_resolution_clock::now();
		stop_time_ = start_time_;
	}

	utils::time_ms Chrono::Stop() {
		stop_time_ = std::chrono::high_resolution_clock::now();
		return TimePassed();
	}
}