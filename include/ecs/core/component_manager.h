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
		using Components = std::unordered_map<size_t, std::unique_ptr<ComponentBase>>;
	private:
		Components components_{};
		static inline size_t type_counter{ 0 };

		template<typename T>
		static size_t getTypeId() {
			static size_t type_id = type_counter++;
			return type_id;
		}
	public:
		ComponentManager() = default;

		ComponentManager(const ComponentManager&) = delete;
		ComponentManager operator=(const ComponentManager&) = delete;

		template<typename T>
		err Register() {
			auto type_key = getTypeId<T>();

			if (auto component = components_.find(type_key); component == components_.end()) {
				components_[type_key] = std::make_unique<CompressedComponentArray<T>>() ;
				return err::ok;
			}
			return err::already_registered;
		}

		template<typename T>
		result<T> Get(Entity entity) const {
			auto type_key = getTypeId<T>();

			lLog(lDebug) << "key: " << type_key << " type: " << typeid(T).name();

			if (auto component = components_.find(type_key); component != components_.end()) {
				auto real_component = std::static_pointer_cast<CompressedComponentArray<T>>(component->first);
				return {};
			}
			return {};
		}

	};
}