#pragma once
#include "EditorWindow.h"
#include "EditorWidget_ComboBox.h"

#include "define_Struct.h"
#include "DefaultShader/CommonStruct.hlsli"

namespace ehw
{
    class Texture;
}

namespace editor
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
        std::shared_ptr<ehw::Texture>        mTextureSrc;
        std::filesystem::path               mTextureDestDir;
    };

}


