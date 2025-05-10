#pragma once
#include "Engine/Game/Component/Collider/Com_Collider3D_Shapes.h"


namespace core
{
    class Com_Collider3D_Cube :
        public Com_Collider3D_Shapes
    {
        CLASS_INFO(Com_Collider3D_Cube, Com_Collider3D_Shapes);
        REGISTER_FACTORY(Com_Collider3D_Cube);
        
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

