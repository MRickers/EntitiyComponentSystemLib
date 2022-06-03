#include <catch2/catch_test_macros.hpp>

#include <ecs/core/entity_manager.h>

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

