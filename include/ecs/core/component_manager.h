#pragma once
#include <optional>
#include <unordered_map>

#include <ecs/core/types.h>

namespace ecs::core {
	class ComponentManager {
		using Components = std::unordered_map<std::string, 
	private:

	public:
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager operator=(const ComponentManager&) = delete;

		ComponentManager() = default;

		template<typename T>
		err Add(Entity entity, const T& component);

		template<typename T>
		std::optional<T> Get(Entity) const;
	};
}