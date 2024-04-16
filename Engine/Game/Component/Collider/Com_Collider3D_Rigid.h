#pragma once
#include "Engine/Game/Component/Collider/iCollider3D.h"

//RigidStatic 또는 RigidDynamic을 가진 Collider
//기본적으로 Static으로 생성, SetDynamic(true)를 할 시 Dynamic으로 변경



//생성 순서
//Init: PxActor 인스턴스 생성
//Awake: PxActor와 Shape 연결
//OnEnable: PxScene에 Actor 연결
//OnDisable: PxScene으로부터 Actor 연결 해제

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
        //void OnEnable() final;
        //void OnDisable() final;
        void Update() final;
        void FinalUpdate() final {}
        void OnDestroy() override;

        //false == static(Default: static)
        void SetDynamic(bool _isDynamic);
        
        void AttachShape(physx::PxShape* const _pxShape);
        void DetachShape(physx::PxShape* const _pxShape);

#pragma region //Dynamic
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
#pragma endregion //Dynamic

    protected:
        physx::PxActor* CreatePxActor() final;

    private:
        bool                        m_isDynamic;
        bool                        m_isTrigger;
        EnumFlags<eFreezeRotationFlag> m_freezeRotationFlag;

        uint32		 m_prevLayer;
        std::vector<physx::PxShape*> m_pxShapes;
    };
}

