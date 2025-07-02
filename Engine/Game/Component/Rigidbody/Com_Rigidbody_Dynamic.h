#pragma once
#include <Engine/Game/Component/Rigidbody/Rigidbody.h>

#include <Engine/define_Enum.h>

namespace core
{
    class Com_Rigidbody_Dynamic :
        public Rigidbody
    {
        CLASS_INFO(Com_Rigidbody_Dynamic, Rigidbody);
        REGISTER_FACTORY(Com_Rigidbody_Dynamic);
        
    public:
        Com_Rigidbody_Dynamic();
        virtual ~Com_Rigidbody_Dynamic();

        void fixed_update() final;

#pragma region //Dynamic
        void SetVelocity(const float3& _velocity);
        float3 GetVelocity();

        void SetLimitVelocity(float maxVelocity);
        float GetLimitVelocity();

        void SetKinematic(bool _enable);

        void SetMass(float _mass);
        float GetMass() const;

        void SetDensity(float _density);

        void SetRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::Enum _flag, bool _enable);
        bool GetRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::Enum _flag) const;
        void SetRigidDynamicLockFlags(physx::PxRigidDynamicLockFlags _flags);
        physx::PxRigidDynamicLockFlags GetLockFlags() const;

        void AddForce(const float3& _force);
        void ClearForce();
#pragma endregion //Dynamic

        

    protected:
        physx::PxRigidActor* CreateRigidbody() final;

    private:
        inline void SetDensityOrMass();

        bool m_isMassMode;

        physx::PxReal m_densityOrMass;
    };

    inline void Com_Rigidbody_Dynamic::SetDensityOrMass()
    {
        if (m_isMassMode)
        {
            SetMass(m_densityOrMass);
        }
        else
        {
            SetDensity(m_densityOrMass);
        }
    }
}
