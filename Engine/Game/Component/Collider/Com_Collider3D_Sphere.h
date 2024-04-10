#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Rigid.h"

namespace ehw
{
    class Com_Collider3D_Sphere :
        public Com_Collider3D_Rigid
    {
    public:
        Com_Collider3D_Sphere();
        virtual ~Com_Collider3D_Sphere();

        void Init() final;

    protected:
    private:
        physx::PxSphereGeometry m_sphereGeometry;

        physx::PxTransform m_offsetPosition;

        float m_offsetRatio_Radius;
    };
}

