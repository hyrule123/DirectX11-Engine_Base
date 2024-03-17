#include "Editor/DebugObject/DebugObject.h"


namespace ehw::editor
{
	DebugObject::DebugObject()
	{

	}
	DebugObject::~DebugObject()
	{

	}

	void DebugObject::FinalUpdate()
	{
		const auto& vecCom = GetComponents();
		for (size_t i = 0; i < vecCom.size(); ++i)
		{
			if (nullptr == vecCom[i])
				continue;

			vecCom[i]->FinalUpdate();
		}
	}
}
