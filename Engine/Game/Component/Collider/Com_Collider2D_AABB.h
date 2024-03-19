#pragma once
#include "Engine/Game/Component/Collider/iCollider2D.h"

namespace ehw
{
    class Com_Collider2D_AABB :
        public iCollider2D
    {
    public:
        Com_Collider2D_AABB();
        virtual ~Com_Collider2D_AABB();

        virtual void Awake() override;
        virtual void Update() override;

        void SetOffsetScale(float2 _scale) { m_offsetScale = _scale; };
        void SyncToTransform(bool _enable) { m_isSyncScaleToTransform = _enable; }

        float2 LeftBottom() const { return m_leftBottom; }
        float2 RightTop() const { return m_rightTop; }

        float Left() const { return m_leftBottom.x; }
        float Bottom() const { return m_leftBottom.y; }
        float Right() const { return m_rightTop.x; }
        float Top() const { return m_rightTop.y; }

    private:
        std::shared_ptr<Com_Transform> m_transform;
        float2 m_offsetScale;
        float2 m_leftBottom;
        float2 m_rightTop;
        bool m_isSyncScaleToTransform;
    };
}
