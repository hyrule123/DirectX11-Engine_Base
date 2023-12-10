#pragma once
#include "EditorResource.h"

namespace editor
{
	class guiTexture : public editor::EditorResource
	{
	public:
		guiTexture();
		virtual ~guiTexture();

		virtual void UpdateUI() override;
	};
}
