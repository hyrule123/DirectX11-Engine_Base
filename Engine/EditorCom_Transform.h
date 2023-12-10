#pragma once
#include "EditorComponent.h"

namespace editor
{
	class guiInspector;
	class EditorCom_Transform : public EditorComponent
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
