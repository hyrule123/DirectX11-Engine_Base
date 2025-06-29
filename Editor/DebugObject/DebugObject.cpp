#include "Editor/DebugObject/DebugObject.h"

#include <Engine/Game/Component/Component.h>

namespace core::editor
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::final_update()
	{
		const auto& vecCom = get_components();
		for (size_t i = 0; i < vecCom.size(); ++i)
		{
			if (nullptr == vecCom[i])
				continue;

			vecCom[i]->final_update();
		}
	}
}
