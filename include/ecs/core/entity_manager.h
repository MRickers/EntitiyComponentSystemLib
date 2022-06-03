#pragma once
#include <queue>
#include <unordered_set>
#include <array>

#include <ecs/core/types.h>

namespace ecs::core {

    class EntityManager {
    private:
        // unused entities
        std::queue<Entity> available_entities_;
        // remember created entities
        std::unordered_set<Entity> living_entities_;
        // array of signatures
        std::array<Signature, MAX_ENTITY_COUNT> signatures_;
        // total entities
        size_t entity_count_;

        err entityExist(Entity entity) const;
    public:
        EntityManager(size_t max_entity_count = MAX_ENTITY_COUNT);
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;

        result<Entity> CreateEntity();
        err DestroyEntity(Entity entity);
        err SetSignature(Entity entity, Signature signature);
        result<Signature> GetSignature(Entity entity) const;
        size_t Count() const;
        bool Empty() const;
    };
}