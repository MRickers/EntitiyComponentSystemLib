#pragma once
#include <unordered_set>

#include <ecs/core/types.h>

namespace ecs::core {
	using time_ms = uint32_t;
	class System {
	private:
		std::unordered_set<Entity> entities_{};
	public:
		virtual ~System() = default;

		err Add(Entity entity) {
			const auto [it, inserted] = entities_.insert(entity);
			if(inserted) return err::ok;
			return err::already_registered;
		}

		err Remove(Entity entity) {
			const auto removed = entities_.erase(entity);
			if (removed) return err::ok;
			return err::not_registered;
		}

		virtual void update(time_ms delta_time) = 0;
	};
}