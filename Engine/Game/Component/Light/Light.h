#pragma once
#include "Engine/Game/Component/Component.h"

namespace ehw
{
    class Light :
        public Component<Light, eComponentCategory::Light>
    {
    public:
        Light(const std::string_view key, eDimensionType _dimensionType);
        virtual ~Light();

        virtual void Render() = 0;

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

