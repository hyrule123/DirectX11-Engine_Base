#pragma once
#include "IResource.h"

namespace ehw
{
    class IAnimation :
        public IResource
    {
    protected:
        IAnimation();
        virtual ~IAnimation();
    };
}

