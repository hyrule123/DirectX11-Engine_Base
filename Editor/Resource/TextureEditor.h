#pragma once
#include "Editor/Resource/ResourceEditor.h"

#include <Engine/Resource/Texture.h>

namespace ehw::editor
{
	class TextureEditor : public ResourceEditor<Texture>
	{
	public:
		TextureEditor();
		virtual ~TextureEditor();

		virtual void UpdateUI() override;
	};
}
