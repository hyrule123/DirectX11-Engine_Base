#pragma once
#include "iResource.h"


namespace ehw
{
	class iShader :
		public iResource
	{
	public:
		iShader(eResourceType _ResType);
		virtual ~iShader();


	};
}


