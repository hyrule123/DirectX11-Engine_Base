#pragma once
#include "guiComponent.h"

namespace editor
{
	class guiInspector;
	class EditorCom_Transform : public guiComponent
	{
	public:
		EditorCom_Transform();
		virtual ~EditorCom_Transform();

		virtual void Update() override;
		virtual void UpdateUI() override;

	private:
		float3 mPosisition;
		float3 mRotation;
		float3 mScale;
	};

}
