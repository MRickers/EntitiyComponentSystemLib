#pragma once
#include <unordered_set>

#include <ecs/core/types.h>

namespace ecs::core {
	class System {
	protected:
		std::unordered_set<Entity> entities_;
	public:
		virtual ~System() = default;

		err Add(Entity entity) {
			if (const auto [it, inserted] = entities_.insert(entity); inserted) {
				return err::ok;
			}
			return err::already_registered;
		}

		err Remove(Entity entity) {
			const auto removed = entities_.erase(entity);
			if (removed) return err::ok;
			return err::not_registered;
		}
	};
}