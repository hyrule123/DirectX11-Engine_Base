#pragma once
#include <Engine/Game/Component/Component.h>
#include <Engine/DefaultShader/Light/Light.hlsli>

namespace core
{
    class Light :
        public Component
    {
        CLASS_INFO(Light, Component);
        BASE_COMPONENT(eComponentOrder::Light);

    public:
        Light(const std::string_view key, eDimensionType _dimensionType);
        virtual ~Light();

        eDimensionType get_dimension_type() const { return m_dimension; }

    private:
        eDimensionType m_dimension;
    };
}

