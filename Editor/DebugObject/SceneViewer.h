#pragma once
#include "Editor/Base/EditorWindow.h"

namespace ehw::editor
{
	class SceneViewer : public EditorWindow
	{
	public:
		SceneViewer();
		virtual ~SceneViewer();


		virtual void update_UI() override;

	private:
	};
}
