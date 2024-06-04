#pragma once
#include <Engine/Scene/GameObject.h>

namespace ehw::editor
{
	class EditorObject : public GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Awake();
		void Update();
		void FinalUpdate();
		void Render();

	private:
		
	};
}
