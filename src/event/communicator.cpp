#include <event/event_bus.h>

namespace ecs::event {
	bool Communicator::AddObserver(std::shared_ptr<IObserver> observer) {
		if (HasObserver(observer)) {
			return false;
		}
		observers_.emplace_back(observer);
		return true;
	}

	bool Communicator::RemoveObserver(std::shared_ptr<IObserver> observer) {
		auto element = std::find_if(observers_.begin(), observers_.end(),
			[&observer](std::shared_ptr<IObserver> o) {
			return o == observer;
		});
		if (element == observers_.end()) {
			return false;
		}
		observers_.erase(element);
		return true;
	}

	bool Communicator::HasObserver(std::shared_ptr<IObserver> observer) {
		return (std::find_if(observers_.begin(), observers_.end(),
			[&observer](std::shared_ptr<IObserver> o) {
			return o == observer; }) != observers_.end());
	}

	void Communicator::Broadcast(const Message& message) {
		for (auto observer : observers_) {
			observer->Notify(message);
		}
	}
}