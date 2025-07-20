#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Engine/Util/SimpleMath.h>

namespace core
{
    class Texture;
}

namespace core
{
    namespace editor
    {
        class EditorUVCalculator :
            public EditorUIWindow
        {
        public:
            EditorUVCalculator();
            virtual ~EditorUVCalculator();

            virtual void init() override;
            virtual void update_UI() override;

        private:
            void update_load_button();
            void update_texture();
            void update_UV_indicator();

        private:
            s_ptr<Texture> m_texture;
            ImVec2                      m_texture_render_size;

            ImVec2 m_cursor_position;
            ImVec2 m_drag_start_position;
            ImVec2 m_drag_end_position;
        };
    }

}


