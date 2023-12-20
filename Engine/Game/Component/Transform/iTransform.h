#pragma once
#include "../iComponent.h"

namespace ehw
{
	class iTransform :
		public Component<iTransform, eComponentCategory::Transform>
	{
	public:
		iTransform();
		virtual ~iTransform();

		virtual void BindData() = 0;

	protected:

	private:

	};
}


