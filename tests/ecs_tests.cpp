#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <ecs/core/entity_manager.h>
#include <ecs/core/component_array.h>
#include <ecs/core/component_layout.h>

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