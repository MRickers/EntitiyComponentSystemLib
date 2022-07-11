#pragma once
#include <memory>

#include <ecs/core/entity_manager.h>
#include <ecs/core/component_manager.h>
#include <ecs/core/system_manager.h>

namespace ecs::core {
    template<typename Events>
    class EntityComponentSystem {

    using EntityManagerPtr =  std::shared_ptr<EntityManager>;
    using ComponentManagerPtr = std::shared_ptr<ComponentManager>;
    using SystemManagerPtr = std::shared_ptr<SystemManager<Events>>;
    private:
        EntityManagerPtr entity_manager_{};
        ComponentManagerPtr component_manager_{};
        SystemManagerPtr system_manager_{};
    public:
        EntityComponentSystem(
            EntityManagerPtr entity_manager = std::make_shared<EntityManager>(),
            ComponentManagerPtr component_manager = std::make_shared<ComponentManager>(),
            SystemManagerPtr system_manager = std::make_shared<SystemManager<Events>>()
        ) :
        entity_manager_(entity_manager),
        component_manager_(component_manager),
        system_manager_(system_manager) {

        }

        result<Entity> CreateEntity() {
            return entity_manager_->CreateEntity();
        }

        void DestroyEntity(const Entity entity) {
            entity_manager_->DestroyEntity(entity);
            component_manager_->DestroyEntity(entity);
            system_manager_->DestroyEntity(entity);
        }

        // Component Methods
        template<typename T>
        err RegisterComponent() {
            return component_manager_->Register<T>();
        }

        template<typename T>
        err AddComponent(Entity entity, const T& component) {
            if(const auto error = component_manager_->Add(entity, component); error != err::ok) {
                return error;
            }
            
            auto result = entity_manager_->GetSignature(entity);
            auto& signature = result.data;
            signature.set(component_manager_->GetComponentType<T>(), true);

            if(const auto error = entity_manager_->SetSignature(entity, signature); error != err::ok) {
                return error;
            }

            system_manager_->SetEntitySignature(entity, signature);

            return err::ok;
        }

        template<typename T>
        err RemoveComponent(Entity entity) {
            if(const auto error = component_manager_->Remove<T>(entity); error != err::ok) {
                return error;
            }

            auto result = entity_manager_->GetSignature(entity);
            auto& signature = result.data;
            signature.set(component_manager_->GetComponentType<T>(), false);

            if(const auto error = entity_manager_->SetSignature(entity, signature); error != err::ok) {
                return error;
            }
            system_manager_->SetEntitySignature(entity, signature);

            return err::ok;
        }

        template<typename T>
        result<T> GetComponent(Entity entity) {
            return component_manager_->Get<T>(entity);
        }

        template<typename T>
        ComponentType GetComponentType() {
            return component_manager_->GetComponentType<T>();
        }

        // System Methods

        template<typename T>
        err RegisterSystem(std::shared_ptr<ecs::core::System> system) {
            if(const auto error = system_manager_->Register<T>(system); error != err::ok) {
                return error;
            }
            if(const auto error = system_manager_->SetSystemSignature<T>(ecs::core::Signature{}); error != err::ok) {
                return error;
            }
            return err::ok;
        }

        template<typename T>
        err RegisterSystem() {
            if(const auto error = system_manager_->Register<T>(); error != err::ok) {
                return error;
            }
            if(const auto error = system_manager_->SetSystemSignature<T>(ecs::core::Signature{}); error != err::ok) {
                return error;
            }
            return err::ok;
        }

        template<typename T>
        err SetSystemSignature(Signature signature) {
            return system_manager_->SetSystemSignature<T>(signature);
        }

        template<typename T>
        err SetEntitySignature(Entity entity, Signature signature) {
            if(const auto error = system_manager_->SetEntitySignature(entity, signature); error != err::ok) {
                return error;
            }
            return err::ok;
        }

    };
}