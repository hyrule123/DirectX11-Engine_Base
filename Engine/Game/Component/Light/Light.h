#pragma once
#include "Engine/Game/Component/BaseComponent.h"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace core
{
#define LIGHT_COMPONENT BaseComponent<Light, eComponentCategory::Light>
    class Light :
        public LIGHT_COMPONENT
    {
        CLASS_INFO(Light, LIGHT_COMPONENT);

    public:
        Light(const std::string_view key, eDimensionType _dimensionType);
        virtual ~Light();

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

