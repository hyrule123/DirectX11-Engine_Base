
#include "EditorCom_Transform.h"

#include "../../Game/Component/Transform/Com_Transform.h"
#include "../../Editor/Inspector/EditorInspector.h"

namespace editor
{

	EditorCom_Transform::EditorCom_Transform()
		: EditorComponent(ehw::eComponentCategory::Transform)
	{
		//SetSize(ImVec2(200.0f, 120.0f));
	}

	EditorCom_Transform::~EditorCom_Transform()
	{

	}

	void EditorCom_Transform::Update()
	{
		if (GetTarget().expired())
		{
			return;
		}

		const auto& tr = GetTarget().lock()->GetComponent<ehw::Com_Transform>();

		m_position = tr->GetLocalPosition();
		m_rotation = tr->GetLocalRotation();
		m_scale = tr->GetLocalScale();
	}

	void EditorCom_Transform::UpdateUI()
	{
		ImGui::DragFloat3("Position", (float*)&m_position);
		ImGui::DragFloat3("Rotation", (float*)&m_rotation);
		ImGui::DragFloat3("Scale", (float*)&m_scale);

		if (false == GetTarget().expired())
		{
			const auto& tr = GetTarget().lock()->GetComponent<ehw::Com_Transform>();

			tr->SetLocalPosition(m_position);
			tr->SetLocalRotation(m_rotation);
			tr->SetLocalScale(m_scale);
		}
	}

}
