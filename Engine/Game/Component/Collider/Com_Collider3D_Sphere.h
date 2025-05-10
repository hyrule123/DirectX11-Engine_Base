#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Shapes.h"

namespace core
{
    class Com_Collider3D_Sphere :
        public Com_Collider3D_Shapes
    {
        CLASS_INFO(Com_Collider3D_Sphere, Com_Collider3D_Shapes);
        REGISTER_FACTORY(Com_Collider3D_Sphere);
        
    public:
        Com_Collider3D_Sphere();
        virtual ~Com_Collider3D_Sphere();

        void init() final;

    protected:
    private:
        physx::PxSphereGeometry m_sphereGeometry;

        physx::PxTransform m_offsetPosition;

        float m_offsetRatio_Radius;
    };
}

