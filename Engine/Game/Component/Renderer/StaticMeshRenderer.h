#pragma once
#include <Engine/Game/Component/Renderer/Renderer.h>

namespace core
{
    class StaticMeshRenderer :
        public Renderer
    {
        CLASS_INFO(StaticMeshRenderer, Renderer);
        REGISTER_FACTORY(StaticMeshRenderer);
        
    public:
        StaticMeshRenderer();
        virtual ~StaticMeshRenderer();

        //virtual void render() override;
    private:
    };
}


