#pragma once
#include "IResource.h"


namespace mh
{
	class IShader :
		public IResource
	{
	public:
		IShader(define::eResourceType _ResType);
		virtual ~IShader();


	};
}


