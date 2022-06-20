#pragma once
#include <optional>
#include <unordered_map>
#include <memory>
#include <typeinfo>

#include <ecs/core/types.h>
#include <ecs/core/component_array.h>
#include <logging/logging.h>

namespace ecs::core {
	class ComponentManager {
		using Components = std::unordered_map<size_t, std::shared_ptr<ComponentBase>>;
	private:
		Components components_{};
		static inline size_t type_counter_{ 0 };

		template<typename T>
		static size_t getTypeId() {
			static size_t type_id = type_counter_++;
			return type_id;
		}
	public:
		ComponentManager() = default;

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager operator=(const ComponentManager&) = delete;

		template<typename T>
		err Register() {
			const auto type_key = getTypeId<T>();

			if (auto component = components_.find(type_key); component == components_.end()) {
				components_[type_key] = std::make_shared<CompressedComponentArray<T>>() ;
				return err::ok;
			}
			return err::already_registered;
		}

		template<typename T>
		result<T> Get(Entity entity) {
			const auto type_key = getTypeId<T>();

			if (auto component = components_.find(type_key); component != components_.end()) {

				auto real_component = std::static_pointer_cast<CompressedComponentArray<T>>(component->second);
				return real_component->Get(entity);
			}
			return {err::not_registered};
		}

		template<typename T>
		err Add(Entity entity, const T& component) {
			const auto type_key = getTypeId<T>();

			if (auto component_it = components_.find(type_key); component_it != components_.end()) {
				auto real_component = std::static_pointer_cast<CompressedComponentArray<T>>(component_it->second);
				return real_component->Add(entity, component);
			}
			return err::not_registered;
		}

		template<typename T>
		err Remove(Entity entity) {
			const auto type_key = getTypeId<T>();

			if (auto component_it = components_.find(type_key); component_it != components_.end()) {
				auto real_component = std::static_pointer_cast<CompressedComponentArray<T>>(component_it->second);
				return real_component->Remove(entity);
			}
			return err::not_registered;
		}

		err DestroyEntity(Entity entity) {
			for (const auto& components : components_) {
				const auto& component = components.second;

				component->DestroyEntity(entity);
			}
			return err::ok;
		}

		template<typename T>
		ComponentType GetComponentType() const {
			return getTypeId<T>();
		}

	};
}