#pragma once
#include <Engine/Game/Component/Collider/Collider3D.h>

//RigidStatic 또는 RigidDynamic을 가진 Collider
//기본적으로 Static으로 생성, SetDynamic(true)를 할 시 Dynamic으로 변경



//생성 순서
//Setting: PxActor 인스턴스 생성
//awake: PxActor와 Shape 연결
//on_enable: PxScene에 Actor 연결
//on_disable: PxScene으로부터 Actor 연결 해제

namespace core
{
    class Collider3D_Shapes :
        public Collider3D
    {
        CLASS_INFO(Collider3D_Shapes, Collider3D);
        REGISTER_FACTORY(Collider3D_Shapes);
        
    public:
        Collider3D_Shapes();

        //상속받아서 사용시
        Collider3D_Shapes(const std::string_view key, eCollider3D_Shape _shape);

        virtual ~Collider3D_Shapes();

        //여기서 Actor에 Shape들을 붙임
        void awake() override;
        void final_update() final {}
        void on_destroy() final;

        void on_layer_change(uint32 _layer) final;

        void set_trigger(bool _enable);
        inline bool is_trigger_mode() const { return m_is_trigger; };

        void add_shape(physx::PxShape* _shape);
        void remove_shape(physx::PxShape* _shape);

    protected:
        //상속받은 클래스에서 Shape를 추가하고자 한다면 사용
        virtual std::vector<physx::PxShape*> add_shape_from_child() { return std::vector<physx::PxShape*>{}; };

    private:
        std::vector<physx::PxShape*> m_px_shapes;

        physx::PxFilterData m_collision_mask;
        physx::PxFilterData m_raycast_mask;

        bool                        m_is_trigger;
    };
}

