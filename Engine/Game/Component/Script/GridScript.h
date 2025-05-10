#pragma once
#include "Engine/Game/Component/Script/Script.h"


namespace core
{
	class Com_Camera;
	class GridScript : public Script
	{
		CLASS_INFO(GridScript, Script);
		
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Awake();
		virtual void update();

		void SetCamera(Com_Camera* camera) { mCamera = camera; }

	private:
		Com_Camera* mCamera;
	};
}
