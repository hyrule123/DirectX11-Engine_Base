#pragma once
#include <Engine/Game/Component/Rigidbody/Rigidbody.h>

#include <Engine/define_Enum.h>

namespace core
{
    class DynamicRigidbody :
        public Rigidbody
    {
        CLASS_INFO(DynamicRigidbody, Rigidbody);
        REGISTER_FACTORY(DynamicRigidbody);
        
    public:
        DynamicRigidbody();
        virtual ~DynamicRigidbody();

        void fixed_update() final;

#pragma region //Dynamic
        void set_velocity(const float3& _velocity);
        float3 get_velocity();

        void set_limit_velocity(float maxVelocity);
        float get_limit_velocity();

        void set_kinematic(bool _enable);

        void set_mass(float _mass);
        float get_mass() const;

        void set_density(float _density);

        void set_rigid_dynamic_lock_flag(physx::PxRigidDynamicLockFlag::Enum _flag, bool _enable);
        bool get_rigid_dynamic_lock_flag(physx::PxRigidDynamicLockFlag::Enum _flag) const;
        void set_rigid_dynamic_lock_flags(physx::PxRigidDynamicLockFlags _flags);
        physx::PxRigidDynamicLockFlags get_lock_flags() const;

        void add_force(const float3& _force);
        void clear_force();
#pragma endregion //Dynamic

        

    protected:
        physx::PxRigidActor* create_rigidbody() final;

    private:
        void set_density_or_mass();

        bool m_isMassMode;

        physx::PxReal m_densityOrMass;
    };
}
