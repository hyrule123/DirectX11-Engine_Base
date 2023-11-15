#pragma once
#include "IRenderer.h"

namespace ehw
{
    class Com_Renderer_Mesh :
        public IRenderer
    {
    public:
        Com_Renderer_Mesh() {};
        virtual ~Com_Renderer_Mesh() {};

        virtual void Render() override;
    private:
    };
}


