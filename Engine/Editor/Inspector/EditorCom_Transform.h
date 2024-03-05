#pragma once
#include "Editor/Inspector/EditorComponent.h"

namespace editor
{
	class EditorInspector;
	class EditorCom_Transform : public EditorComponent
	{
	public:
		EditorCom_Transform();
		virtual ~EditorCom_Transform();

		virtual void Update() override;
		virtual void UpdateUI() override;

	private:
		float3 m_position;
		Quaternion m_rotation;
		float3 m_scale;
	};

}
