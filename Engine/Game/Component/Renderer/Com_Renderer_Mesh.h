#pragma once
#include <Engine/Game/Component/Renderer/Renderer.h>

namespace core
{
    class Com_Renderer_Mesh :
        public Renderer
    {
        CLASS_INFO(Com_Renderer_Mesh, Renderer);
        REGISTER_FACTORY(Com_Renderer_Mesh);
        
    public:
        Com_Renderer_Mesh();
        Com_Renderer_Mesh(const std::string_view key);
        virtual ~Com_Renderer_Mesh();

        //virtual void render() override;
    private:
    };
}


