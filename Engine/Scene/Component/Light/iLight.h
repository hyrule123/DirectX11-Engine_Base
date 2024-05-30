#pragma once
#include "Engine/Scene/Component/iComponent.h"

namespace ehw
{
    class iLight :
        public Component<iLight, eComponentCategory::Light>
    {
    public:
        iLight(eDimensionType _dimensionType);
        virtual ~iLight();

        virtual void Render() = 0;

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

