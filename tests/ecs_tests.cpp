#include <sstream>

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <ecs/core/entity_manager.h>
#include <ecs/core/component_array.h>
#include <ecs/core/component_layout.h>
#include <ecs/core/component_manager.h>
#include <ecs/core/system_manager.h>

TEST_CASE("EntityManager create", "[entitymanager]") {
    ecs::core::EntityManager manager;

    REQUIRE(manager.Count() == 0);
}

TEST_CASE("EntityManager create/destroy Entity", "[entitymanager]") {
    ecs::core::EntityManager manager;

    REQUIRE(manager.CreateEntity().data == 0);
    REQUIRE(manager.CreateEntity().data == 1);
    REQUIRE(manager.CreateEntity().data == 2);
    REQUIRE(manager.CreateEntity().data == 3);
    REQUIRE(manager.Count() == 4);

    REQUIRE(manager.DestroyEntity(0) == ecs::core::err::ok);
    REQUIRE(manager.Count() == 3);
    REQUIRE(manager.DestroyEntity(1) == ecs::core::err::ok);
    REQUIRE(manager.Count() == 2);
    REQUIRE(manager.DestroyEntity(2) == ecs::core::err::ok);
    REQUIRE(manager.Count() == 1);
    REQUIRE(manager.DestroyEntity(3) == ecs::core::err::ok);
    REQUIRE(manager.Count() == 0);
}

TEST_CASE("EntityManager destroy invalid Entity", "[entitymanager]") {
    ecs::core::EntityManager manager;

    REQUIRE(manager.DestroyEntity(0) == ecs::core::err::no_entity);
}

TEST_CASE("EntityManager set/get Signature", "[entitymanager]") {
    ecs::core::EntityManager manager;

    const auto e1 = manager.CreateEntity();
    const auto e2 = manager.CreateEntity();

    REQUIRE(manager.SetSignature(e1.data, ecs::core::Signature{1}) == ecs::core::err::ok);
    REQUIRE(manager.SetSignature(e2.data, ecs::core::Signature{2}) == ecs::core::err::ok);
    REQUIRE(manager.GetSignature(e1.data).data == ecs::core::Signature{1});
    REQUIRE(manager.GetSignature(e2.data).data == ecs::core::Signature{2});
}

TEST_CASE("EntityManager Signature invalid", "[entitymanager]") {
    ecs::core::EntityManager manager;

    REQUIRE(manager.SetSignature(0, ecs::core::Signature{}) == ecs::core::err::no_entity);
    REQUIRE(manager.GetSignature(0).error == ecs::core::err::no_signature);
}

TEST_CASE("EntityManager empty", "[entitymanager]") {
    ecs::core::EntityManager manager(2);

    manager.CreateEntity();
    manager.CreateEntity();

    REQUIRE(manager.Count() == 2);
    REQUIRE(manager.Empty());
}

TEST_CASE("ComponentArray add/get", "[componentarray]") {
    ecs::core::CompressedComponentArray<int> array;

    SECTION("Add Get") {
        REQUIRE(array.Add(0, 1) == ecs::core::err::ok);
        REQUIRE(array.Get(0).data == 1);
    }
    SECTION("Get not added") {
        REQUIRE(array.Get(0).error == ecs::core::err::no_entity);
        REQUIRE(array.Get(0).data == 0);
    }
    SECTION("Add Get multiple") {
        REQUIRE(array.Add(0, 1) == ecs::core::err::ok);
        REQUIRE(array.Add(1, 2) == ecs::core::err::ok);
        REQUIRE(array.Add(2, 3) == ecs::core::err::ok);
        REQUIRE(array.Add(3, 4) == ecs::core::err::ok);

        REQUIRE(array.Get(0).data == 1);
        REQUIRE(array.Get(2).data == 3);
        REQUIRE(array.Get(3).data == 4);
        REQUIRE(array.Get(1).data == 2);
    }
    SECTION("Add/Remove") {
        REQUIRE(array.Add(0, 1) == ecs::core::err::ok);
        REQUIRE(array.Add(1, 2) == ecs::core::err::ok);
        REQUIRE(array.Add(2, 3) == ecs::core::err::ok);
        REQUIRE(array.Add(3, 4) == ecs::core::err::ok);

        REQUIRE(array.Remove(1) == ecs::core::err::ok);
        REQUIRE(array.Get(1).error == ecs::core::err::no_entity);
        REQUIRE(array.Get(3).data == 4);

        REQUIRE(array.Remove(0) == ecs::core::err::ok);
        REQUIRE(array.Get(0).error == ecs::core::err::no_entity);
        REQUIRE(array.Get(2).data == 3);
    }
}

TEST_CASE("Component Array struct", "[componentarray]") {
    SECTION("Add temporary") {
        struct Foo {
            int x{0};
            int y{1};
        };
        ecs::core::CompressedComponentArray<Foo> array;

        REQUIRE(array.Add(10, Foo()) == ecs::core::err::ok);
        REQUIRE(array.Get(10).error == ecs::core::err::ok);
        REQUIRE(array.Get(10).data.x == 0);
        REQUIRE(array.Get(10).data.y == 1);
    }
}

TEST_CASE("Component Array Layout", "[layout]") {
    ecs::core::Compressor layout;

    SECTION("Empty layout") {
        REQUIRE(layout.Size() == 0);
        REQUIRE(layout.Get(0).error == ecs::core::err::no_entity);
    }
    SECTION("Add") {
        REQUIRE(layout.Add(0).data == 0);
        REQUIRE(layout.Add(1).data == 1);
        REQUIRE(layout.Add(2).data == 2);
        REQUIRE(layout.Add(3).data == 3);
        REQUIRE(layout.Size() == 4);
    }
    SECTION("Add/Get") {
        REQUIRE(layout.Add(35).data == 0);
        REQUIRE(layout.Add(48).data == 1);
        REQUIRE(layout.Add(100).data == 2);
        REQUIRE(layout.Add(12).data == 3);
        REQUIRE(layout.Size() == 4);

        REQUIRE(layout.Get(100).data == 2);
        REQUIRE(layout.Get(35).data == 0);
        REQUIRE(layout.Get(12).data == 3);
        REQUIRE(layout.Get(48).data == 1);
    }

    SECTION("Add/Remove") {
        REQUIRE(layout.Add(35).data == 0);
        REQUIRE(layout.Add(48).data == 1);
        REQUIRE(layout.Add(100).data == 2);
        REQUIRE(layout.Add(12).data == 3);
        REQUIRE(layout.Size() == 4);

        REQUIRE(layout.Remove(48).data == 1);
        REQUIRE(layout.Remove(35).data == 0);
        REQUIRE(layout.Size() == 2);
        REQUIRE(layout.Get(100).data == 0);
        REQUIRE(layout.Get(12).data == 1);
        REQUIRE(layout.Get(35).error == ecs::core::err::no_entity);
        REQUIRE(layout.Get(48).error == ecs::core::err::no_entity);
    }

    SECTION("Add/Remove/Add") {
        REQUIRE(layout.Add(1).data == 0);
        REQUIRE(layout.Add(2).data == 1);
        REQUIRE(layout.Add(3).data == 2);
        REQUIRE(layout.Add(4).data == 3);
        REQUIRE(layout.Size() == 4);

        REQUIRE(layout.Remove(2).data == 1);
        REQUIRE(layout.Remove(1).data == 0);
        //  0 1 2 3
        // |3|4| | |
        REQUIRE(layout.Size() == 2);
        REQUIRE(layout.Get(3).data == 0);
        REQUIRE(layout.Get(4).data == 1);
        REQUIRE(layout.Get(1).error == ecs::core::err::no_entity);
        REQUIRE(layout.Get(2).error == ecs::core::err::no_entity);

        REQUIRE(layout.Add(2).data == 2);
        //  0 1 2 3
        // |3|4|2| |
        REQUIRE(layout.Remove(4).data == 1);
        //  0 1 2 3
        // |3|2| | |
        REQUIRE(layout.Get(3).data == 0);
        REQUIRE(layout.Get(2).data == 1);
    }
}

TEST_CASE("Register component", "[component manager]") {
    struct Foo {
        int x;
    };
    struct Bar {
        std::string a{""};
        bool b{false};
    };

    ecs::core::ComponentManager manager;

    SECTION("Register") {
        REQUIRE(manager.Register<int>() == ecs::core::err::ok);
        REQUIRE(manager.Register<float>() == ecs::core::err::ok);
        REQUIRE(manager.Register<Foo>() == ecs::core::err::ok);
        REQUIRE(manager.Register<char>() == ecs::core::err::ok);
        REQUIRE(manager.Register<std::string>() == ecs::core::err::ok);
        REQUIRE(manager.Register<Bar>() == ecs::core::err::ok);
    }
    SECTION("Already registered") {
        REQUIRE(manager.Register<Foo>() == ecs::core::err::ok);
        REQUIRE(manager.Register<Foo>() == ecs::core::err::already_registered);
        REQUIRE(manager.Register<std::string>() == ecs::core::err::ok);
    }
}

TEST_CASE("Add components", "[component manager]") {
    struct Bar {
        int x{0};
        int y{1};
    };

    ecs::core::ComponentManager manager;

    SECTION("Add simple") {
        manager.Register<std::string>();
        REQUIRE(manager.Add<std::string>(5, "Hello there") == ecs::core::err::ok);
    }
    SECTION("Add not registered") {
        REQUIRE(manager.Add<double>(1000, 1.337) == ecs::core::err::not_registered);
    }
    SECTION("Add different components") {
        REQUIRE(manager.Register<Bar>() == ecs::core::err::ok);
        REQUIRE(manager.Register<int>() == ecs::core::err::ok);

        REQUIRE(manager.Add(10, Bar()) == ecs::core::err::ok);
        REQUIRE(manager.Add(10, 35) == ecs::core::err::ok);
        REQUIRE(manager.Get<Bar>(10).error == ecs::core::err::ok);
        REQUIRE(manager.Get<Bar>(10).data.x == 0);
        REQUIRE(manager.Get<Bar>(10).data.y == 1);
    }
    SECTION("Benchmark add") {
        manager.Register<std::string>();
        BENCHMARK("Add a small component") {
            return manager.Add<std::string>(1000, "General Kenobi");
        };
        BENCHMARK("Add a large component") {
            return manager.Add<std::string>(2000, "Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test ");
        };
    }
}

TEST_CASE("Get components", "[component manager]") {
    ecs::core::ComponentManager manager;

    SECTION("Get simple") {
        REQUIRE(manager.Register<int>() == ecs::core::err::ok);
        REQUIRE(manager.Get<int>(0).error == ecs::core::err::no_entity);
    }
    SECTION("Get not registered") {
        REQUIRE(manager.Get<char>(100).error == ecs::core::err::not_registered);
    }
    SECTION("Add/Get") {
        REQUIRE(manager.Register<std::string>() == ecs::core::err::ok);
        REQUIRE(manager.Add<std::string>(100, "Hello there") == ecs::core::err::ok);
        REQUIRE(manager.Add<std::string>(1, "General Kenobi") == ecs::core::err::ok);
        REQUIRE(manager.Add<std::string>(12, "Foo") == ecs::core::err::ok);
        REQUIRE(manager.Add<std::string>(1000, "Bar") == ecs::core::err::ok);

        REQUIRE(manager.Get<std::string>(100).error == ecs::core::err::ok);
        REQUIRE(manager.Get<std::string>(100).data == "Hello there");
        REQUIRE(manager.Get<std::string>(1).error == ecs::core::err::ok);
        REQUIRE(manager.Get<std::string>(1).data == "General Kenobi");
        REQUIRE(manager.Get<std::string>(12).error == ecs::core::err::ok);
        REQUIRE(manager.Get<std::string>(12).data == "Foo");
        REQUIRE(manager.Get<std::string>(1000).error == ecs::core::err::ok);
        REQUIRE(manager.Get<std::string>(1000).data == "Bar");
    }

    SECTION("Benchmark Get") {
        manager.Register<std::string>();
        manager.Add<std::string>(1000, "General Kenobi");
        manager.Add<std::string>(2000, "Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test Test ");
        BENCHMARK("Get a small component") {
            return manager.Get<std::string>(1000);
        };
        BENCHMARK("Get a large component") {
            return manager.Get<std::string>(2000);
        };
    }
}

TEST_CASE("Add Entity", "[system]") {
    class BarSystem : public ecs::core::System {
    public:
        virtual void update(ecs::core::time_ms delta_time) override {}
    };
    BarSystem system;

    REQUIRE(system.Add(0) == ecs::core::err::ok);
    REQUIRE(system.Add(0) == ecs::core::err::already_registered);
    REQUIRE(system.Remove(0) == ecs::core::err::ok);
    REQUIRE(system.Remove(0) == ecs::core::err::not_registered);
}

TEST_CASE("Register System", "[system manager]") {
    struct TestSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };
    struct FooSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };

    ecs::core::SystemManager manager;

    SECTION("Register") {
        REQUIRE(manager.Register<TestSystem>() == ecs::core::err::ok);
    }
    SECTION("Already registered") {
        REQUIRE(manager.Register<TestSystem>() == ecs::core::err::ok);
        REQUIRE(manager.Register<TestSystem>() == ecs::core::err::already_registered);
    }
    SECTION("Inject system") {
        const auto system = std::make_shared<FooSystem>();
        REQUIRE(manager.Register(system) == ecs::core::err::ok);
    }
}

TEST_CASE("Set system signature", "[system manager]") {
    struct TestSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };
    struct FooSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };

    ecs::core::SystemManager manager;

    SECTION("Add Siganture") {
        REQUIRE(manager.Register<TestSystem>() == ecs::core::err::ok);
        REQUIRE(manager.SetSystemSignature<TestSystem>(ecs::core::Signature()) == ecs::core::err::ok);
    }
    SECTION("Not Registered System") {
        REQUIRE(manager.SetSystemSignature<TestSystem>(ecs::core::Signature()) == ecs::core::err::not_registered);
    }
}

TEST_CASE("Set Entity Signature" "[system manager]") {
    struct TestSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };
    struct FooSystem : ecs::core::System {
        virtual void update(ecs::core::time_ms delta_time) override {}
    };

    ecs::core::SystemManager manager;

    SECTION("Add Entity Signature not registered") {
        REQUIRE(manager.SetEntitySignature(0, ecs::core::Signature()) == ecs::core::err::not_registered);
    }
    SECTION("Add invalid system") {
        auto system = std::shared_ptr<TestSystem>();
        REQUIRE(manager.Register(system) == ecs::core::err::invalid_argument);
    }
    SECTION("Add Entity Signaure") {
        auto system = std::make_shared<FooSystem>();
        REQUIRE(manager.Register(system) == ecs::core::err::ok);
        REQUIRE(manager.SetSystemSignature<FooSystem>(ecs::core::Signature(1)) == ecs::core::err::ok);
        REQUIRE(manager.SetEntitySignature(0, ecs::core::Signature(1)) == ecs::core::err::ok);
        REQUIRE(manager.SetEntitySignature(0, ecs::core::Signature(1)) == ecs::core::err::already_registered);
        REQUIRE(manager.SetEntitySignature(0, ecs::core::Signature(0)) == ecs::core::err::ok);
        REQUIRE(manager.SetEntitySignature(0, ecs::core::Signature(0)) == ecs::core::err::not_registered);
        REQUIRE(manager.DestroyEntity(0) == ecs::core::err::ok);
    }
}