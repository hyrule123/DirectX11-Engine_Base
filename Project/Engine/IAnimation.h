#pragma once
#include "IResource.h"

namespace mh
{
    class IAnimation :
        public IResource
    {
    protected:
        IAnimation();
        virtual ~IAnimation();
    };
}

