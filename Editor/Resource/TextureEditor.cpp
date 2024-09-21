#include "Editor/Resource/TextureEditor.h"


namespace ehw::editor
{
	TextureEditor::TextureEditor()
		: ResourceEditor(::ehw::strKey::path::directory::resource::Texture)
	{

	}

	TextureEditor::~TextureEditor()
	{

	}

	void TextureEditor::UpdateUI()
	{
		if (GetTarget().expired())
		{
			return;
		}

		const auto& targetTex = GetTarget().lock();

		ImGui::Image(targetTex->GetSRV().Get(), ImVec2(150.0f, 150.0f));

		const std::string_view key = targetTex->get_resource_name();

		ImGui::Text("Key	"); ImGui::SameLine();
		ImGui::InputText("##TexKey"
			, (char*)key.data(), key.size(), ImGuiInputTextFlags_ReadOnly);

		//ImGui::Text("Path	"); ImGui::SameLine();
		//ImGui::InputText("##TexPath"
		//	, (char*)path.data(), path.size(), ImGuiInputTextFlags_ReadOnly);

		int width = (int)targetTex->GetWidth();
		int height = (int)targetTex->GetHeight();


		char wBuffer[256] = "";
		_itoa_s(width, wBuffer, 10);

		ImGui::Text("Width	"); ImGui::SameLine();
		ImGui::InputText("##TexWidth"
			, wBuffer, 256, ImGuiInputTextFlags_ReadOnly);

		char hBuffer[256] = "";
		_itoa_s(height, hBuffer, 10);

		ImGui::Text("Height	"); ImGui::SameLine();
		ImGui::InputText("##TexHeight"
			, hBuffer, 256, ImGuiInputTextFlags_ReadOnly);
	}
}
