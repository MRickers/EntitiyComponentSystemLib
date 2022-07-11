#pragma once
#include <any>

#include <ecs/core/types.h>

namespace ecs::event {
	// Holds the data
	class Message {
	private:
		std::any data_{};
		ecs::core::Entity sender_{ 0 };
		ecs::core::Entity receiver_{ 0 };
	public:
		Message() = default;
		Message(ecs::core::Entity sender, ecs::core::Entity receiver) : data_(), sender_(sender), receiver_(receiver){}

		template<typename T>
		Message(const T& data) : data_(data) {}

		template<typename T>
		T GetData() const {
			if (data_.has_value()) {
				return std::any_cast<T>(data_);
			}
			return T();
		}

		template<typename T>
		void SetData(const T& data) {
			data_ = data;
		}

		std::pair<ecs::core::Entity, ecs::core::Entity> GetInvolved() const {
			return { sender_, receiver_ };
		}
	};

	class IObserver {
	public:
		virtual ~IObserver() {}
		virtual void Notify(const Message& message) = 0;
	};

}