#pragma once
#include <Editor/Base/EditorUIWindow.h>

namespace core::editor
{
	class SceneViewer : public EditorUIWindow
	{
	public:
		SceneViewer();
		virtual ~SceneViewer();


		virtual void update_UI() override;

	private:
	};
}
