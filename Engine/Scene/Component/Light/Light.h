#pragma once
#include "Engine/Scene/Component/Component.h"

namespace ehw
{
    class Light :
        public Component<Light, eComponentCategory::Light>
    {
    public:
        Light(eDimensionType _dimensionType);
        virtual ~Light();

        virtual void Render() = 0;

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

