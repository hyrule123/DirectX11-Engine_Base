#pragma once
#include <Editor/Base/EditorUIWindow.h>

#include <Engine/Common.h>

namespace core
{
    class Texture;
}

namespace core::editor
{
    class EditorNormalConverter :
        public EditorUIWindow
    {
    public:
        EditorNormalConverter();
        virtual ~EditorNormalConverter();

        virtual void init() override;
        virtual void update_UI() override;

    private:
        void src_texture_UI_update();
        void dest_texture_UI_update();
        void copy_texture_UI_update();

        void reset();

        
    private:
        s_ptr<Texture>        m_src_texture;
        std::filesystem::path               m_dest_texture_dir;
    };

}


