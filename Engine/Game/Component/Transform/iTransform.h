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

		virtual eResult Serialize(JsonSerializer& _ser) override { return eResult::Success; }
		virtual eResult DeSerialize(const JsonSerializer& _ser) override { return eResult::Success; }

		virtual void BindData() = 0;

	protected:

	private:

	};
}


