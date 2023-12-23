
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

		mPosisition = tr->GetRelativePos();
		mRotation = tr->GetRelativeRotXYZ();
		mScale = tr->GetRelativeScale();
	}

	void EditorCom_Transform::UpdateUI()
	{
		ImGui::DragFloat3("Position", (float*)&mPosisition);
		ImGui::DragFloat3("Rotation", (float*)&mRotation);
		ImGui::DragFloat3("Scale", (float*)&mScale);

		if (false == GetTarget().expired())
		{
			const auto& tr = GetTarget().lock()->GetComponent<ehw::Com_Transform>();

			tr->SetRelativePos(mPosisition);
			tr->SetRelativeRotXYZ(mRotation);
			tr->SetRelativeScale(mScale);
		}
	}

}
