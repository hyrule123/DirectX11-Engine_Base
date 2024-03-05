#pragma once
#include "Game/Component/Renderer/iRenderer.h"

namespace ehw
{
    class Com_Renderer_Mesh :
        public iRenderer
    {
    public:
        Com_Renderer_Mesh() {};
        virtual ~Com_Renderer_Mesh() {};

        virtual void Render() override;
    private:
    };
}


