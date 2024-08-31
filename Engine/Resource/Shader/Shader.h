#pragma once
#include "Engine/Resource/Resource.h"

namespace ehw
{
	class Shader :
		public Resource
	{
	public:
		Shader(const std::string_view key, const std::type_info& _typeInfo);
		virtual ~Shader();
	};
}


