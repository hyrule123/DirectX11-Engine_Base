#pragma once
#include <Engine/Game/Component/Collider/Collider3D_Shapes.h>


namespace core
{
    class Collider3D_Cube :
        public Collider3D_Shapes
    {
        CLASS_INFO(Collider3D_Cube, Collider3D_Shapes);
        REGISTER_FACTORY(Collider3D_Cube);
        
    public:
        Collider3D_Cube();
        virtual ~Collider3D_Cube();

        std::vector<physx::PxShape*> add_shape_from_child() final;

        inline void set_offset_position(const physx::PxVec3& _offsetPos) { m_offset_position.p = _offsetPos; }
    private:
        physx::PxBoxGeometry m_offset_half_scale;
        physx::PxTransform m_offset_position;

        float3 m_offset_scale;
    };
}

