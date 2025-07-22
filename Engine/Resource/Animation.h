#pragma once
#include <Engine/Resource/Resource.h>

namespace core
{
    BASE_RESOURCE(Animation);
    class Animation :
        public Resource
    {
    protected:
        Animation();
        virtual ~Animation();
    };
}

