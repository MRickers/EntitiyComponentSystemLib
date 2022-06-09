#pragma once
#include <array>
#include <unordered_map>

#include <ecs/core/types.h>

namespace ecs::core {
	class ComponentBase {
	public:
		virtual ~ComponentBase() = default;
		virtual void DestroyEntity(Entity entity) = 0;
	};

	template<typename T>
	class ComponentArray : public ComponentBase {
	private:
		std::array<T, MAX_ENTITY_COUNT> components_{};
		std::unordered_map<Entity, std::size_t> entity_to_index_{};
		std::unordered_map<std::size_t, Entity> index_to_entity_{};
		std::size_t size_{ 0 };
	public:
		err Add(Entity entity, const T& component) {
			if (const auto new_index = size_; new_index < MAX_ENTITY_COUNT) {
				entity_to_index_[entity] = new_index;
				index_to_entity_[new_index] = entity;
				components_[new_index] = component;
				size_++;

				return err::ok;
			}
			return err::entity_limit;
		}

		result<T> Get(Entity entity) const {
			if (const auto index = entity_to_index_.find(entity); index != entity_to_index_.end()) {
				return result<T>(components_[index->second]);
			}
			return result<T>({}, err::no_entity);
		}

		err Remove(Entity entity) {
			if (const auto index_of_removed_entity = entity_to_index_.find(entity); index_of_removed_entity != entity_to_index_.end()) {
				const auto index_of_last_entity = size_ - 1;
				components_[index_of_removed_entity->second] = components_[index_of_last_entity];
				Entity last_entity = index_to_entity_[index_of_last_entity];
				entity_to_index_[last_entity] = index_of_removed_entity->second;
				index_to_entity_[index_of_removed_entity->second] = last_entity;
				entity_to_index_.erase(entity);
				index_to_entity_.erase(index_of_last_entity);
				size_--;
				return err::ok;
			}
			return err::no_entity;
		}

		virtual void DestroyEntity([[maybe_unused]]Entity entity) override {

		}
	};
}