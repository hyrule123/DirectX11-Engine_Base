#pragma once
#include "../../Editor/Base/EditorWindow.h"

namespace editor
{
	class EditorGame : public EditorWindow
	{
	public:
		EditorGame();
		virtual ~EditorGame();


		virtual void UpdateUI() override;

	private:
	};
}
