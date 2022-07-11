#pragma once
#include <queue>

#include <ecs/core/types.h>

namespace ecs::event {
	template<typename Event>
	class EventQueue {
	private:
		std::queue<Event> queue_{};
	public:
		void Enqueue(Event evnt) {
			queue_.push(evnt);
		}

		ecs::core::result<Event> Dequeue() {
			if (queue_.empty()) return {ecs::core::err::empty};
			const auto evnt = queue_.front();
			queue_.pop();
			return { evnt };
		}

		void Clear() {
			while (!queue_.empty()) {
				queue_.pop();
			}
		}

		size_t Size() const {
			return queue_.size();
		}

		bool Empty() const {
			return queue_.empty();
		}
	};
}