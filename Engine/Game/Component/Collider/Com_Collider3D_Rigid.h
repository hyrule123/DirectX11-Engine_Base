#pragma once
#include "Engine/Game/Component/Collider/iCollider3D.h"

//강체(하나 또는 여러개)를 가진 단순 Collider
//생성 순서
//Init: PxActor, Shape
//OnEnable: PxActor에 Shape 연결
//OnDisable: PxActor로부터 Shape 연결 해제

namespace ehw
{
    class Com_Collider3D_Rigid :
        public iCollider3D
    {
    public:
        Com_Collider3D_Rigid();

        //상속받아서 사용시
        Com_Collider3D_Rigid(eCollider3D_Shape _shape);

        virtual ~Com_Collider3D_Rigid();

        //여기서 Actor에 Shape들을 붙임
        void OnEnable() final;
        void OnDisable() final;
        void Update() final;
        void FinalUpdate() final {}
        void OnDestroy() override;

        //false == static(Default: static)
        inline void SetDynamic(bool _isDynamic) { m_isDynamic = _isDynamic; }
        
        void AttachShape(const PhysXSharedPtr<physx::PxShape>& _pxShape);
        void DetachShape(physx::PxShape* _pxShape);

        void SetVelocity(const float3& _velocity);
        float3 GetVelocity();

        void SetLimitVelocity(float maxVelocity);
        float GetLimitVelocity() const;

        void SetTrigger(bool _enable);
        inline bool IsTrigger() const { return m_isTrigger; }

        void SetKinematic(bool enable);

        void SetMass(float _mass);

        void SetFreezeRotation(EnumFlags<eFreezeRotationFlag> _flag);
        EnumFlags<eFreezeRotationFlag> GetFreezeRotation() const { return m_freezeRotationFlag; }
        inline bool hasFlag(eFreezeRotationFlag flag) const { return m_freezeRotationFlag.isSet(flag); }

        void AddForce(const float3& _force);
        void ClearForce();
        

    protected:
        physx::PxActor* CreateActor() final;

    private:
        bool			            m_isDynamic;
        bool                        m_isTrigger;
        EnumFlags<eFreezeRotationFlag> m_freezeRotationFlag;

        uint32		 m_prevLayer;

        std::vector<PhysXSharedPtr<physx::PxShape>> m_pxShapes;
    };
}

