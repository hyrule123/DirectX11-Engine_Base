
#include "EditorInspector.h"


#include "../../Manager/RenderManager.h"
#include "../../Editor/Resource/EditorTexture.h"

#include "EditorCom_Transform.h"
#include "EditorCom_Renderer.h"

namespace editor
{
	EditorInspector::EditorInspector()
		: EditorWindow(strKey::Inspector)
		, mTargetResource()
	{
		//컴포넌트의 경우 수동 관리
		mGuiComponents.resize((int)ehw::eComponentCategory::END);
		mGuiResources.resize((int)ehw::eResourceType::END);

		mGuiComponents[(int)ehw::eComponentCategory::Transform] = new EditorCom_Transform;
		mGuiComponents[(int)ehw::eComponentCategory::Transform]->SetSize(ImVec2(0.f, 100.f));

		mGuiComponents[(int)ehw::eComponentCategory::Renderer] = new EditorCom_Renderer;
		mGuiComponents[(int)ehw::eComponentCategory::Renderer]->SetSize(ImVec2(0.f, 100.f));

		mGuiResources[(int)ehw::eResourceType::Texture] = new EditorTexture;
	}

	EditorInspector::~EditorInspector()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
				delete mGuiComponents[i];
		}

		for (size_t i = 0; i < mGuiResources.size(); ++i)
		{
			if (mGuiResources[i])
				delete mGuiResources[i];
		}
	}

	void EditorInspector::Init()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
				mGuiComponents[i]->InitRecursive();
		}

		for (size_t i = 0; i < mGuiResources.size(); ++i)
		{
			if (mGuiResources[i])
				mGuiResources[i]->InitRecursive();
		}
	}

	void EditorInspector::Update()
	{
		mTargetGameObject = ehw::RenderManager::GetInspectorGameObject();

		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			if (mGuiComponents[i])
			{
				if (mTargetGameObject)
				{
					mGuiComponents[i]->SetTarget(mTargetGameObject);
					mGuiComponents[i]->Update();
				}
					
			}
		}

		
		for (size_t i = 0; i < mGuiResources.size(); ++i)
		{

			if (mGuiResources[i])
			{
				if (mTargetResource)
				{
					mGuiResources[i]->SetTarget(mTargetResource);
					mGuiResources[i]->Update();
				}
			}
				
		}
	}

	void EditorInspector::UpdateUI()
	{
		for (size_t i = 0; i < mGuiComponents.size(); ++i)
		{
			IndicatorButton(ehw::strKey::eComponentCategory_String[(UINT)i]);

			if (mTargetGameObject && mGuiComponents[i])
				mGuiComponents[i]->InternalUpdate();
		}

		for (size_t i = 0; i < mGuiResources.size(); ++i)
		{
			IndicatorButton(ehw::strKey::ArrResName[i]);

			if (mTargetResource && mGuiResources[i])
				mGuiResources[i]->InternalUpdate();
		}
	}

	void EditorInspector::IndicatorButton(const char* _strButtonName)
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
