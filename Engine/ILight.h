#pragma once
#include "IComponent.h"

namespace ehw
{
    class ILight :
        public IComponent
    {
    public:
        ILight(eDimensionType _dimensionType);
        virtual ~ILight();

        virtual void Render() = 0;

        eDimensionType GetDimensionType() const { return mDimension; }

    private:
        eDimensionType mDimension;
    };
}

