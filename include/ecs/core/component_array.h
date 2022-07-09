#pragma once
#include <array>
#include <unordered_map>

#include <ecs/core/types.h>
#include <ecs/core/component_layout.h>

namespace ecs::core {
	class ComponentBase {
	public:
		virtual ~ComponentBase() = default;
		virtual void DestroyEntity(Entity entity) = 0;
	};

	template<typename T, typename MemoryLayout>
	class ComponentArray : public ComponentBase {
	private:
		std::array<T, MAX_ENTITY_COUNT> components_{};
		MemoryLayout memory_layout_{};
	public:
		err Add(Entity entity, const T& component) {
			const result<size_t> result = memory_layout_.Add(entity);

			if(result.error != err::ok) {
				return result.error;
			}
			
			const auto new_index = result.data;
			components_[new_index] = component;
			return err::ok;
		}

		result<T> Get(Entity entity) const {
			const auto result = memory_layout_.Get(entity);
			if(result.error != err::ok) {
				return {result.error};
			}
			return {components_[result.data]};
		}

		err Remove(Entity entity) {
			const result<size_t> result = memory_layout_.Remove(entity);

			if(result.error != err::ok) {
				return result.error;
			}

			const auto index_last_entity = memory_layout_.Size();
			const auto index_removed_entity = result.data;

			components_[index_removed_entity] = components_[index_last_entity];
			return err::ok;
		}

		virtual void DestroyEntity(Entity entity) override {
			Remove(entity);
		}
	};
	template<typename T>
	using CompressedComponentArray = ComponentArray<T, ecs::core::Compressor>;
}