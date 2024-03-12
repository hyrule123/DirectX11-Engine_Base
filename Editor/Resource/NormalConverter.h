#pragma once
#include "Editor/Base/EditorWindow.h"

#include <Engine/CommonStruct.h>

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

        virtual void Init() override;
        virtual void UpdateUI() override;

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


