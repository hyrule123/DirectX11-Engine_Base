#include "PCH_Engine.h"
#include "guiCom_Transform.h"

#include "Com_Transform.h"
#include "guiInspector.h"

namespace gui
{
	guiCom_Transform::guiCom_Transform()
		: guiComponent(ehw::eComponentCategory::Transform)
	{
		//SetSize(ImVec2(200.0f, 120.0f));
	}

	guiCom_Transform::~guiCom_Transform()
	{

	}

	void guiCom_Transform::Update()
	{
		ehw::Com_Transform* tr = GetTarget()->GetComponent<ehw::Com_Transform>();

		mPosisition = tr->GetRelativePos();
		mRotation = tr->GetRelativeRotXYZ();
		mScale = tr->GetRelativeScale();
	}

	void guiCom_Transform::UpdateUI()
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
