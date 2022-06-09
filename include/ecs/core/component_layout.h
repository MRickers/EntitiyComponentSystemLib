#pragma once
#include <unordered_map>

#include <ecs/core/types.h>

namespace ecs::core {
	// Interface for Component Memory layout logic
	class Layout {
	public:
		virtual ~Layout() = default;

		virtual result<size_t> Add(Entity entity) = 0;
		virtual result<size_t> Get(Entity entity) const = 0;
		virtual result<size_t> Remove(Entity entity) = 0;
        virtual size_t Size() const = 0;
	};

	class Compressor : public Layout {
	private:
		std::unordered_map<Entity, size_t> entity_to_index_{};
		std::unordered_map<size_t, Entity> index_to_entity_{};
		size_t size_{ 0 };
	public:
		virtual result<size_t> Add(Entity entity) override;
		virtual result<size_t> Get(Entity entity) const override;
		virtual result<size_t> Remove(Entity entity) override;
        virtual size_t Size() const override;
	};

}