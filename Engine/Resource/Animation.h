#pragma once
#include "Engine/Resource/Resource.h"

namespace ehw
{
    BASE_RESOURCE(Animation);
    class Animation :
        public Resource
    {
    protected:
        Animation(const std::string_view key);
        virtual ~Animation();
    };
}

