#include <string>

#include <catch2/catch_test_macros.hpp>

#include <event/event.h>
#include <event/event_bus.h>
#include <event/event_queue.h>

class MockObserver : public ecs::event::IObserver {
private:
	std::string info{"Empty"};
public:
	virtual void Notify(const ecs::event::Message& message) override {
		const auto item = message.GetData<std::string>();
		info = item;
	}

	std::string Info() const {
		return info;
	}
};


TEST_CASE("set/get", "[Message]") {
	struct Test {
	public:
		float x{ 0 };
		int y{ 1 };
	};

	ecs::event::Message msg;

	SECTION("Add int") {
		msg.SetData(0);
		REQUIRE(msg.GetData<int>() == 0);
	}
	SECTION("Add string") {
		msg.SetData(std::string{ "Hello There" });
		REQUIRE(msg.GetData<std::string>() == "Hello There");
	}
	SECTION("Add struct") {
		msg.SetData(Test());
		REQUIRE(msg.GetData<Test>().x == 0);
		REQUIRE(msg.GetData<Test>().y == 1);
	}
}

TEST_CASE("Communicator HasObserver", "[communicator]") {
	ecs::event::Communicator c;
	auto observer = std::make_shared<MockObserver>();

	SECTION("Empty") {
		REQUIRE(c.HasObserver(observer) == false);
	}
	SECTION("Observer added") {
		REQUIRE(c.AddObserver(observer) == true);
		REQUIRE(c.HasObserver(observer) == true);
	}
	SECTION("Add multiple") {
		const auto obs1 = std::make_shared<MockObserver>();
		const auto obs2 = std::make_shared<MockObserver>();
		REQUIRE(c.AddObserver(obs1) == true);
		REQUIRE(c.AddObserver(obs2) == true);
		REQUIRE(c.HasObserver(observer) == false);
		REQUIRE(c.HasObserver(obs1) == true);
		REQUIRE(c.HasObserver(obs2) == true);
	}
	SECTION("Remove") {
		REQUIRE(c.AddObserver(observer) == true);
		REQUIRE(c.HasObserver(observer) == true);
		REQUIRE(c.RemoveObserver(observer) == true);
		REQUIRE(c.RemoveObserver(observer) == false);
		REQUIRE(c.HasObserver(observer) == false);
	}
}

TEST_CASE("EventBus", "[eventbus]") {
	enum class TestEvents {
		foo,
	};
	ecs::event::EventBus<TestEvents> events;

	SECTION("Subscribe event") {
		const auto observer = std::make_shared<MockObserver>();

		REQUIRE(events.Subscribe(TestEvents::foo, observer) == true);
	}
	SECTION("Dispatch") {
		auto observer = std::make_shared<MockObserver>();
		ecs::event::Message message;

		message.SetData<std::string>("Hello");

		REQUIRE(events.Subscribe(TestEvents::foo, observer) == true);
		REQUIRE(observer->Info() == "Empty");
		events.Dispatch(TestEvents::foo, message);
		REQUIRE(observer->Info() == "Hello");
	}
	SECTION("Unsubscribe") {
		auto observer = std::make_shared<MockObserver>();

		ecs::event::Message message;

		message.SetData<std::string>("Hello");

		REQUIRE(events.Subscribe(TestEvents::foo, observer) == true);
		REQUIRE(observer->Info() == "Empty");
		REQUIRE(events.Unsubscribe(TestEvents::foo, observer) == true);
		events.Dispatch(TestEvents::foo, message);
		REQUIRE(observer->Info() == "Empty");
	}
}

TEST_CASE("EventQueue", "[eventqueue]") {
	enum class Events {
		foo,
		bar,
	};

	ecs::event::EventQueue<Events> queue;

	SECTION("Add and dequeue") {
		queue.Enqueue(Events::foo);
		const auto evnt = queue.Dequeue();
		REQUIRE(evnt.error == ecs::core::err::ok);
		REQUIRE(evnt.data == Events::foo);
		REQUIRE(queue.Empty());
	}
	SECTION("Dequeue empty") {
		const auto evnt = queue.Dequeue();
		REQUIRE(evnt.error == ecs::core::err::empty);
	}
	SECTION("Clear queue") {
		queue.Enqueue(Events::bar);
		queue.Enqueue(Events::foo);
		REQUIRE(queue.Size() == 2);
		REQUIRE(!queue.Empty());
		queue.Clear();
		REQUIRE(queue.Empty());
	}
}