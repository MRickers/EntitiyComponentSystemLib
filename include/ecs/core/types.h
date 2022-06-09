#pragma once
#include <bitset>

namespace ecs::core {

    static const size_t MAX_COMPONENTS = 64;
    static const size_t MAX_ENTITY_COUNT = 0x1000;
    using Entity = size_t;
	using Signature = std::bitset<MAX_COMPONENTS>;
    using ComponentType = uint16_t;

    enum class err {
        ok,
        no_entity,
        no_signature,

        entity_limit,

    };

    enum class components {
        position,

    };

    enum class systems {
        renderer,
    };

    template<typename T>
    struct result {
        result(T d, err err=err::ok) : data(d), error(err){}
        result() : data(), error(err::ok){}
        result(err err) : data(), error(err){};

        T data;
        err error;
    };
}