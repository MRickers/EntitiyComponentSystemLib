#include <sstream>

#include <catch2/catch_test_macros.hpp>

#include <ecs/core/entity_manager.h>
#include <ecs/core/component_array.h>

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
    ecs::core::ComponentArray<int> array;

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
    SECTION("Add check entity index") {
        array.Add(0, 111);
        array.Add(1, 111);
        array.Add(2, 111);
        array.Add(3, 111);

        const auto entity_index = array.GetEntityIndex();
        REQUIRE(entity_index.at(0) == 0);
        REQUIRE(entity_index.at(1) == 1);
        REQUIRE(entity_index.at(2) == 2);
        REQUIRE(entity_index.at(3) == 3);
    }
    SECTION("Add/Remove check entity index") {
        array.Add(0, 111);
        array.Add(1, 111);
        array.Add(2, 111);
        array.Add(3, 111);

        array.Remove(1);

        const auto entity_index = array.GetEntityIndex();
        REQUIRE(entity_index.at(0) == 0);
        REQUIRE(entity_index.at(3) == 1);
        REQUIRE(entity_index.at(2) == 2);
    }
}