#pragma once
#include "Engine/Resource/Resource.h"

namespace ehw
{
	class Shader :
		public Resource
	{
	public:
		Shader(const std::string_view key);
		virtual ~Shader();
	};
}


