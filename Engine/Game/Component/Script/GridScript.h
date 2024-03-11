#pragma once
#include "Engine/Game/Component/Script/iScript.h"


namespace ehw
{
	class Com_Camera;
	class GridScript : public iScript
	{
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Awake();
		virtual void Update();

		void SetCamera(const std::shared_ptr<Com_Camera>& camera) { mCamera = camera; }

	private:
		std::shared_ptr<Com_Camera> mCamera;
	};
}
