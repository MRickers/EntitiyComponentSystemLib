#pragma once
#include <clock_chrono.h>

namespace utils {

	<typename Clock>
	class Timer {
	private:
		bool started_{ false };
		Clock clock_;
	public:
		Timer() : started_(false), clock_() {}

		void Start() {
			clock_.Start();
		}

		time_ms Stop() {
			started_ = false;
			return clock_.Stop();
		}

		time_ms TimePassed() const {
			return clock_.TimePassed();
		}
		bool IsStarted() const {
			return started_;
		}
	};

	using ChronoTimer = Timer<utils::clock::Chrono>;
}