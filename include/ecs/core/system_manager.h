#pragma once
#include <unordered_map>
#include <memory>

#include <ecs/core/system.h>
#include <logging/logging.h>

namespace ecs::core {
	class SystemManager {
	private:
		std::unordered_map<size_t, Signature> signatures_;
		std::unordered_map<size_t, std::shared_ptr<System>> systems_;

		static inline size_t type_counter_{ 0 };
		template<typename T>
		static size_t getTypeId() {
			static size_t type_id = type_counter_++;
			return type_id;
		}
	public:
		template<typename T>
		err Register() {
			const auto type_id = getTypeId<T>();

			if (const auto element = systems_.find(type_id); element == systems_.end()) {
				systems_[type_id] = std::make_shared<T>();
				return err::ok;
			}
			return err::already_registered;
		}

		template<typename T>
		err Register(std::shared_ptr<T> system) {
			const auto type_id = getTypeId<T>();

			if (const auto element = systems_.find(type_id); element == systems_.end()) {
				systems_[type_id] = system;
				return err::ok;
			}
			return err::already_registered;
		}

		template<typename T>
		err SetSystemSignature(Signature signature) {
			const auto type_id = getTypeId<T>();

			if (systems_.find(type_id) != systems_.end()) {
				signatures_.insert_or_assign(type_id, signature);
				return err::ok;
			}
			return err::not_registered;
		}

		err SetEntitySignature(Entity entity, Signature signature) {
			for (const auto& [type_id, system] : systems_) {
				
				if (const auto& element = signatures_.find(type_id); element != signatures_.end()) {
					const auto& system_signature = element->second;

					if ((signature & system_signature) == system_signature) {
						if (const auto error = system->Add(entity); error != err::ok) {
							lLog(lWarn) << "Add entity " << entity << " failed";
							return error;
						}
					}
					else {
						if (const auto error = system->Remove(entity); error != err::ok) {
							lLog(lWarn) << "Remove entity " << entity << " failed";
						}
					}
				}
			}
			return err::ok;
		}

		err DestroyEntity(Entity entity) {
			for (auto& [type_id, system] : systems_) {
				if (const auto error = system->Remove(entity); error != err::ok) {
					lLog(lWarn) << "Destroying Entity " << entity << "failed";
					return err::ok;
				}
			}
			return err::ok;
		}

	};
}