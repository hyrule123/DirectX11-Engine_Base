#pragma once
#include <Editor/Resource/ResourceEditor.h>

#include <Engine/Resource/Texture.h>

namespace core::editor
{
	class TextureEditor : public ResourceEditor<Texture>
	{
		CLASS_INFO(TextureEditor, ResourceEditor<Texture>);
	public:
		TextureEditor();
		virtual void init() override;

		virtual ~TextureEditor();

		virtual void update_UI() override;
	};
}
