#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include "DefaultShader/SH_CommonStruct.hlsli"

namespace mh
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

    private:
        std::shared_ptr<mh::Texture> mTextureSrc;
        std::shared_ptr<mh::Texture> mTextureDest;
    };
}


