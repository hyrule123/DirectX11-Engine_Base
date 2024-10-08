#pragma once
#include "Editor/Base/EditorWindow.h"

#include <Engine/Common.h>

namespace ehw
{
    class Texture;
}

namespace ehw::editor
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
        std::shared_ptr<Texture>        mTextureSrc;
        std::filesystem::path               mTextureDestDir;
    };

}


