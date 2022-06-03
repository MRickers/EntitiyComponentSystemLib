#include <algorithm>
#include <ecs/core/entity_manager.h>
#include <logging/logging.h>


namespace ecs::core {
    EntityManager::EntityManager(size_t max_entity_count) :
    available_entities_(),
    living_entities_(),
    signatures_(),
    entity_count_(0) {
        for (Entity entity = 0; entity < (max_entity_count-1 > MAX_ENTITY_COUNT ? MAX_ENTITY_COUNT : max_entity_count); entity++) {
            available_entities_.push(entity);
        }
    }

    result<Entity> EntityManager::CreateEntity() {
        if (available_entities_.empty()) {
            return result<Entity>({}, err::no_entity);
        }
        Entity id = available_entities_.front();
        living_entities_.insert(id);
        available_entities_.pop();
        entity_count_++;

        return result<Entity>(id, err::ok);
    }

    err EntityManager::DestroyEntity(Entity entity) {
        if(const auto removed = living_entities_.erase(entity); removed) {
            available_entities_.push(entity);
            signatures_[entity].reset();
            --entity_count_;

            return err::ok;
        }

        return err::no_entity;
    }

    err EntityManager::SetSignature(Entity entity, Signature signature) {
        if(const auto found = living_entities_.find(entity); found != living_entities_.end()) {
            signatures_[entity] = signature;

            return err::ok;
        }
        return err::no_entity;
    }

    result<Signature> EntityManager::GetSignature(Entity entity) const {
        if(const auto found = living_entities_.find(entity); found != living_entities_.end()) {
            return result<Signature>(signatures_[entity]);
        }
        return result<Signature>({}, err::no_signature);
    }

    size_t EntityManager::Count() const {
        return living_entities_.size();
    }

    bool EntityManager::Empty() const {
        return available_entities_.empty();
    }
}