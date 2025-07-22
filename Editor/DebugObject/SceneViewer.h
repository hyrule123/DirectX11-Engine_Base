#pragma once
#include <Editor/Base/EditorUIWindow.h>

namespace core::editor
{
	class SceneViewer : public EditorUIWindow
	{
		CLASS_INFO(SceneViewer, EditorUIWindow);
	public:
		SceneViewer();
		virtual void init() override;

		virtual ~SceneViewer();


		virtual void update_UI() override;

	private:
	};
}
