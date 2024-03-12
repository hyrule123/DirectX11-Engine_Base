#include "Editor/Inspector/InspectorBase.h"

#include <Engine/Manager/RenderManager.h>
#include "Editor/Resource/TextureEditor.h"

#include "Editor/Inspector/Inspector_Com_Transform.h"
#include "Editor/Inspector/Inspector_Com_Renderer.h"

namespace ehw::editor
{
	InspectorBase::InspectorBase()
		: EditorWindow(strKey::Inspector)
		, mTargetResource()
	{
		//컴포넌트의 경우 수동 관리
		mGuiComponents.resize((int)eComponentCategory::END);
		//mGuiResources.resize((int)eResourceType::END);

		mGuiComponents[(int)eComponentCategory::Transform] = new Inspector_Com_Transform;
		mGuiComponents[(int)eComponentCategory::Transform]->SetSize(ImVec2(0.f, 100.f));

		mGuiComponents[(int)eComponentCategory::Renderer] = new Inspector_Com_Renderer;
		mGuiComponents[(int)eComponentCategory::Renderer]->SetSize(ImVec2(0.f, 100.f));

		//mGuiResources[(int)eResourceType::Texture] = new TextureEditor;
	}

	InspectorBase::~InspectorBase()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
			{
				delete mGuiComponents[i];
			}
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//	{
		//		delete mGuiResources[i];
		//	}
		//}
	}

	void InspectorBase::Init()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
				mGuiComponents[i]->InitRecursive();
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if (mGuiResources[i])
		//		mGuiResources[i]->InitRecursive();
		//}
	}

	void InspectorBase::Update()
	{

		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
			{
				if (false == mTargetGameObject.expired())
				{
					mGuiComponents[i]->SetTarget(mTargetGameObject);
					mGuiComponents[i]->Update();
				}
					
			}
		}

		
		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{

		//	if (mGuiResources[i])
		//	{
		//		if (false == mTargetGameObject.expired())
		//		{
		//			mGuiResources[i]->SetTarget(mTargetResource);
		//			mGuiResources[i]->Update();
		//		}
		//	}
		//		
		//}
	}

	void InspectorBase::UpdateUI()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			IndicatorButton(::ehw::strKey::eComponentCategory_String[(UINT)i]);

			if (false == mTargetGameObject.expired() && mGuiComponents[i])
			{
				mGuiComponents[i]->InternalUpdate();
			}
		}

		//for (size_t i = 0; i < mGuiResources.size(); ++i)
		//{
		//	if ((false == mTargetResource.expired()) && mGuiResources[i])
		//	{
		//		mGuiResources[i]->InternalUpdate();
		//	}
		//}
	}

	void InspectorBase::IndicatorButton(const char* _strButtonName)
	{
		ImGui::PushID(0);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.8f, 0.8f));

		ImGui::Button(_strButtonName);
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
}
