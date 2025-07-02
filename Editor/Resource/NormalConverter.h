#pragma once
#include <Editor/Base/EditorWindow.h>

#include <Engine/Common.h>

namespace core
{
    class Texture;
}

namespace core::editor
{
    class EditorNormalConverter :
        public EditorWindow
    {
    public:
        EditorNormalConverter();
        virtual ~EditorNormalConverter();

        virtual void init() override;
        virtual void update_UI() override;

    private:
        void SrcTextureUpdate();
        void DestTextureUpdate();
        void CopyTextureUpdate();

        void Reset();

        
    private:
        s_ptr<Texture>        mTextureSrc;
        std::filesystem::path               mTextureDestDir;
    };

}


