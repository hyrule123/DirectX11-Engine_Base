#pragma once
#include "Engine/Game/Component/Component.h"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace ehw
{
    class Light :
        public Component<Light, eComponentCategory::Light>
    {
        
    public:
        Light(const std::string_view key, eDimensionType _dimensionType);
        virtual ~Light();

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

