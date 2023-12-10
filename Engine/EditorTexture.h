#pragma once
#include "EditorResource.h"

namespace editor
{
	class EditorTexture : public EditorResource
	{
	public:
		EditorTexture();
		virtual ~EditorTexture();

		virtual void UpdateUI() override;
	};
}
