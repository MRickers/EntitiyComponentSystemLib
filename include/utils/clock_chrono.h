#pragma once
#include <chrono>

#include <utils/types.h>

namespace utils::clock {

	class Chrono {
	private:
		std::chrono::high_resolution_clock::time_point start_time_{};
		std::chrono::high_resolution_clock::time_point stop_time_{};

	public:
		Chrono();

		utils::time_ms TimePassed() const;
		void Start();
		utils::time_ms Stop();

	};
}