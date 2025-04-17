#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Shapes.h"


namespace core
{
    class Com_Collider3D_Cube :
        public Com_Collider3D_Shapes
    {
        CLASS_NAME(Com_Collider3D_Cube);
        REGISTER_INSTANCE_DEFAULT(Com_Collider3D_Cube);
        
    public:
        Com_Collider3D_Cube();
        virtual ~Com_Collider3D_Cube();

        std::vector<physx::PxShape*> AddShapeFromChild() final;

        inline void SetOffsetPosition(const physx::PxVec3& _offsetPos) { m_offsetPosition.p = _offsetPos; }
    private:
        physx::PxBoxGeometry m_offsetHalfScale;
        physx::PxTransform m_offsetPosition;

        float3 m_offsetScale;
    };
}

