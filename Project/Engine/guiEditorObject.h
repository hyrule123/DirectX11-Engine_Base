#pragma once

#include "GameObject.h"

namespace gui
{
	class EditorObject : public mh::GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();

		void Init();
		void Update();
		void FixedUpdate();
		void Render();

	private:
		
	};
}
