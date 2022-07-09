#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

#include <event/event.h>

namespace ecs::event {
	class Communicator {
		using ObserverContainer = std::vector<std::shared_ptr<IObserver>>;
	private:
		ObserverContainer observers_{};
	public:
		bool AddObserver(std::shared_ptr<IObserver> observer);
		bool RemoveObserver(std::shared_ptr<IObserver> observer);
		bool HasObserver(std::shared_ptr<IObserver> observer);
		void Broadcast(const Message& message);
	};

	template<typename Event>
	class EventBus {
		using Subscriptions = std::unordered_map<Event, Communicator>;
	private:
		Subscriptions subscriptions_;

	public:

		bool Subscribe(Event evnt, std::shared_ptr<IObserver> observer) {
			return subscriptions_[evnt].AddObserver(observer);
		}

		bool Unsubscribe(Event evnt, std::shared_ptr<IObserver> observer) {
			return subscriptions_[evnt].RemoveObserver(observer);
		}

		void Dispatch(Event evnt, const Message& message) {
			subscriptions_[evnt].Broadcast(message);
		}
	};
}