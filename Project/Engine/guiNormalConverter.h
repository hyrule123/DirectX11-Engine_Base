#pragma once
#include "guiWindow.h"
#include "guiComboBox.h"
#include "DefaultShader/SH_CommonStruct.hlsli"

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
        void LoadTexture();

    private:
        
    };
}


