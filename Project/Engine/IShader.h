#pragma once
#include "IResource.h"


namespace ehw
{
	class IShader :
		public IResource
	{
	public:
		IShader(define::eResourceType _ResType);
		virtual ~IShader();


	};
}


