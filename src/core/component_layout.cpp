#include <ecs/core/component_layout.h>

namespace ecs::core {
    result<size_t> Compressor::Add(Entity entity) {
        if (const auto new_index = size_; new_index < MAX_ENTITY_COUNT) {
            entity_to_index_[entity] = new_index;
            index_to_entity_[new_index] = entity;
            size_++;

            return {new_index};
        }
        return {err::entity_limit};
    }

    result<size_t> Compressor::Get(Entity entity) const {
        if (const auto index = entity_to_index_.find(entity); index != entity_to_index_.end()) {
            return {index->second};
        }
        return {err::no_entity};
    }

    result<size_t> Compressor::Remove(Entity entity) {
        if (const auto index_of_removed_entity = entity_to_index_.find(entity); index_of_removed_entity != entity_to_index_.end()) {
            const auto index_of_last_entity = size_ - 1;
            Entity last_entity = index_to_entity_[index_of_last_entity];
            entity_to_index_[last_entity] = index_of_removed_entity->second;
            index_to_entity_[index_of_removed_entity->second] = last_entity;
            entity_to_index_.erase(entity);
            index_to_entity_.erase(index_of_last_entity);
            size_--;
            return {index_of_removed_entity->second};
        }
        return {err::no_entity};
    }

    size_t Compressor::Size() const {
        return size_;
    }

}