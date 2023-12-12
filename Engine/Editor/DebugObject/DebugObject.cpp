#include "../../PCH_Engine.h"

#include "DebugObject.h"


namespace editor
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::InternalUpdate()
	{
		const auto& vecCom = GetComponents();
		for (size_t i = 0; i < vecCom.size(); ++i)
		{
			if (nullptr == vecCom[i])
				continue;

			vecCom[i]->InternalUpdate();
		}
	}
}
