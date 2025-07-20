#pragma once
#include <Engine/Game/Component/Collider/Collider3D_Shapes.h>

namespace core
{
    class Collider3D_Capsule :
        public Collider3D_Shapes
    {
        CLASS_INFO(Collider3D_Capsule, Collider3D_Shapes);
        REGISTER_FACTORY(Collider3D_Capsule);
        
    public:
        Collider3D_Capsule();
        virtual ~Collider3D_Capsule();

    protected:
        void init() override;
    private:
        physx::PxCapsuleGeometry m_capsule_geometry;

        physx::PxTransform m_offset_position;

        float m_offset_ratio_radius;
        float m_offset_ratio_half_height;
    };
}

