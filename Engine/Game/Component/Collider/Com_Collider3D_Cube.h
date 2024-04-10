#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Rigid.h"


namespace ehw
{
    class Com_Collider3D_Cube :
        public Com_Collider3D_Rigid
    {
    public:
        Com_Collider3D_Cube();
        virtual ~Com_Collider3D_Cube();

        void Init() final;

        inline void SetOffsetPosition(const physx::PxVec3& _offsetPos) { m_offsetPosition.p = _offsetPos; }
    private:
        physx::PxBoxGeometry m_pxBoxGeometry;
        physx::PxTransform m_offsetPosition;

        float3 m_offsetScale;
    };
}

