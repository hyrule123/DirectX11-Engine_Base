#pragma once
#include "iComponent.h"

namespace ehw
{
	class iTransform :
		public iComponent
	{
	public:
		iTransform();
		virtual ~iTransform();

		virtual void BindData() = 0;

	protected:

	private:

	};
}


