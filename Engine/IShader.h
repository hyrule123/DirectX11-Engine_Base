#pragma once
#include "IResource.h"


namespace ehw
{
	class IShader :
		public IResource
	{
	public:
		IShader(eResourceType _ResType);
		virtual ~IShader();


	};
}


