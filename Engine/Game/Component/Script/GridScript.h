#pragma once
#include "iScript.h"
#include "../Camera/Com_Camera.h"

namespace ehw
{
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
