#pragma once
#include <Engine/Game/Component/Collider/Collider3D_Shapes.h>

namespace core
{
    class Collider3D_Sphere :
        public Collider3D_Shapes
    {
        CLASS_INFO(Collider3D_Sphere, Collider3D_Shapes);
        REGISTER_FACTORY(Collider3D_Sphere);
        
    public:
        Collider3D_Sphere();
        virtual ~Collider3D_Sphere();

        void init() final;

    protected:
    private:
        physx::PxSphereGeometry m_sphere_geometry;

        physx::PxTransform m_offset_position;

        float m_offset_ratio_radius;
    };
}

