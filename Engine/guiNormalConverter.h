#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include "DefaultShader/CommonStruct.hlsli"

namespace ehw
{
    class Texture;
}

namespace gui
{
    class guiNormalConverter :
        public guiWindow
    {
    public:
        guiNormalConverter();
        virtual ~guiNormalConverter();

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


