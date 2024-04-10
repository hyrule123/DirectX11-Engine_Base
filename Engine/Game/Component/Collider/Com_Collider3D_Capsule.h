#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Rigid.h"

namespace ehw
{
    class Com_Collider3D_Capsule :
        public Com_Collider3D_Rigid
    {
    public:
        Com_Collider3D_Capsule();
        virtual ~Com_Collider3D_Capsule();

    protected:
        void Init() override;
    private:
        physx::PxCapsuleGeometry m_capsuleGeometry;

        physx::PxTransform m_offsetPosition;

        float m_offsetRatio_Radius;
        float m_offsetRatio_HalfHeight;
    };
}

