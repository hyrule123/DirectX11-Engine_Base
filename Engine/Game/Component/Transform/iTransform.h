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

		//TODO: 여기 구현 할것.
		virtual eResult Serialize_Json(JsonSerializer* _ser) const override { return eResult::Success; }
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) override { return eResult::Success; }

		virtual void BindData() = 0;

	protected:

	private:

	};
}


