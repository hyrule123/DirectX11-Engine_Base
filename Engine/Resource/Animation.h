#pragma once
#include <Engine/Resource/Resource.h>

namespace core
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

