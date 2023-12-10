#pragma once
#include "guiResource.h"

namespace editor
{
	class guiTexture : public editor::guiResource
	{
	public:
		guiTexture();
		virtual ~guiTexture();

		virtual void UpdateUI() override;
	};
}
