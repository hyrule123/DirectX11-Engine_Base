#pragma once
#include "Engine/Game/Component/Renderer/Renderer.h"

namespace ehw
{
    class Com_Renderer_Mesh :
        public Renderer
    {
        REGISTER_CLASS_INSTANCE_ABLE(Com_Renderer_Mesh);
        
    public:
        Com_Renderer_Mesh();
        Com_Renderer_Mesh(const std::string_view key);
        virtual ~Com_Renderer_Mesh();

        virtual void Render() override;
    private:
    };
}


