#pragma once
#include "Engine/Scene/Component/Renderer/Renderer.h"

namespace ehw
{
    class Com_Renderer_Mesh :
        public Renderer
    {
    public:
        Com_Renderer_Mesh() {};
        virtual ~Com_Renderer_Mesh() {};

        virtual void Render() override;
    private:
    };
}


