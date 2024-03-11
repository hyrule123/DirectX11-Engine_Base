#pragma once
#include "Engine/Resource/iResource.h"

namespace ehw
{
	class iShader :
		public iResource
	{
	public:
		iShader(const std::type_info& _typeInfo);
		virtual ~iShader();
	};
}


