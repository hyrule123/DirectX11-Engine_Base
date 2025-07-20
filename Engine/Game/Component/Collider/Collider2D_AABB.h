#pragma once
#include <Engine/Game/Component/Collider/Collider2D.h>

namespace core
{
    class Collider2D_AABB :
        public Collider2D
    {
        CLASS_INFO(Collider2D_AABB, Collider2D);
        REGISTER_FACTORY(Collider2D_AABB);
    public:
        Collider2D_AABB();
        virtual ~Collider2D_AABB();

        virtual void update_shape() final;
        virtual void frame_end() final;

        void set_offset_scale(float2 _scale) { m_offset_scale = _scale; m_b_offset_scale_updated = true; };
        void sync_to_transform(bool _enable) { m_b_sync_scale_to_transform = _enable; }

        float2 left_bottom() const { return m_left_bottom; }
        float2 right_top() const { return m_right_top; }

        float left() const { return m_left_bottom.x; }
        float bottom() const { return m_left_bottom.y; }
        float right() const { return m_right_top.x; }
        float top() const { return m_right_top.y; }

        virtual MATRIX get_collider_matrix() final;

    private:
        float2 m_offset_scale;
        bool m_b_offset_scale_updated;


        float2 m_left_bottom;
        float2 m_right_top;
        bool m_b_sync_scale_to_transform;
    };
}

