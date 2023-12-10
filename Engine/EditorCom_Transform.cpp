#include "PCH_Engine.h"
#include "EditorCom_Transform.h"

#include "Com_Transform.h"
#include "guiInspector.h"

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
		ehw::Com_Transform* tr = GetTarget()->GetComponent<ehw::Com_Transform>();

		mPosisition = tr->GetRelativePos();
		mRotation = tr->GetRelativeRotXYZ();
		mScale = tr->GetRelativeScale();
	}

	void EditorCom_Transform::UpdateUI()
	{
		ImGui::DragFloat3("Position", (float*)&mPosisition);
		ImGui::DragFloat3("Rotation", (float*)&mRotation);
		ImGui::DragFloat3("Scale", (float*)&mScale);

		if (GetTarget())
		{
			ehw::Com_Transform* tr = GetTarget()->GetComponent<ehw::Com_Transform>();

			tr->SetRelativePos(mPosisition);
			tr->SetRelativeRotXYZ(mRotation);
			tr->SetRelativeScale(mScale);
		}
	}

}
