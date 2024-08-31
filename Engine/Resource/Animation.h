#pragma once
#include "Engine/Resource/Resource.h"

namespace ehw
{
    class Animation :
        public Resource
    {
    protected:
        Animation(const std::string_view key, const std::type_info& _typeInfo);
        virtual ~Animation();
    };
}

