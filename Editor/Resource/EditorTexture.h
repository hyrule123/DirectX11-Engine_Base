#pragma once
#include "Resource/EditorResource.h"

#include "Resource/Texture.h"

namespace editor
{
	class EditorTexture : public EditorResource<ehw::Texture>
	{
	public:
		EditorTexture();
		virtual ~EditorTexture();

		virtual void UpdateUI() override;
	};
}
