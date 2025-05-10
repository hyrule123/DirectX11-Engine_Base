#pragma once
#include "Engine/Game/Component/Collider/Collider3D.h"

//RigidStatic 또는 RigidDynamic을 가진 Collider
//기본적으로 Static으로 생성, SetDynamic(true)를 할 시 Dynamic으로 변경



//생성 순서
//Setting: PxActor 인스턴스 생성
//Awake: PxActor와 Shape 연결
//OnEnable: PxScene에 Actor 연결
//OnDisable: PxScene으로부터 Actor 연결 해제

namespace core
{
    class Com_Collider3D_Shapes :
        public Collider3D
    {
        CLASS_INFO(Com_Collider3D_Shapes, Collider3D);
        REGISTER_FACTORY(Com_Collider3D_Shapes);
        
    public:
        Com_Collider3D_Shapes();

        //상속받아서 사용시
        Com_Collider3D_Shapes(const std::string_view key, eCollider3D_Shape _shape);

        virtual ~Com_Collider3D_Shapes();

        //여기서 Actor에 Shape들을 붙임
        void Awake() override;
        void final_update() final {}
        void OnDestroy() final;

        void OnLayerChange(uint32 _layer) final;

        void SetTrigger(bool _enable);
        inline bool IsTrigger() const { return m_isTrigger; };

        void AddShape(physx::PxShape* _shape);
        void RemoveShape(physx::PxShape* _shape);

    protected:
        //상속받은 클래스에서 Shape를 추가하고자 한다면 사용
        virtual std::vector<physx::PxShape*> AddShapeFromChild() { return std::vector<physx::PxShape*>{}; };

    private:
        std::vector<physx::PxShape*> m_pxShapes;

        physx::PxFilterData m_collisionMask;
        physx::PxFilterData m_raycastMask;

        bool                        m_isTrigger;
    };
}

