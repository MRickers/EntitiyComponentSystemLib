#pragma once
#include <any>

namespace ecs::event {
	// Holds the data
	class Message {
	private:
		std::any data_;
	public:
		Message() = default;

		template<typename T>
		Message(const T& data) : data_(data) {}

		template<typename T>
		T GetData() const {
			return std::any_cast<T>(data_);
		}

		template<typename T>
		void SetData(const T& data) {
			data_ = data;
		}
	};

	class IObserver {
	public:
		virtual ~IObserver() {}
		virtual void Notify(const Message& message) = 0;
	};

}