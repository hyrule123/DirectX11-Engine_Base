#pragma once
#include "Resource/iResource.h"

namespace ehw
{
    class iAnimation :
        public iResource
    {
    protected:
        iAnimation(const std::type_info& _typeInfo);
        virtual ~iAnimation();
    };
}

