#pragma once
#include <Engine/Game/GameObject.h>

namespace ehw::editor
{
	class EditorObject : public GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Awake();
		void Update();
		void InternalUpdate();
		void Render();

	private:
		
	};
}
