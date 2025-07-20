#include "NormalConverter.h"

#include <Engine/define_Enum.h>

#include <Editor/Widget/ComboBoxWidget.h>

#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/Util/define_Util.h>

#include <Engine/Resource/Shader/ComputeShaders/NormalConvertShader.h>
#include <Engine/DefaultShader/NormalConverter/NormalConverter.hlsli>

namespace core::editor
{
	EditorNormalConverter::EditorNormalConverter()
		: EditorUIWindow("Normal Converter")
		, m_src_texture{}
		, m_dest_texture_dir{}
	{
	}
	EditorNormalConverter::~EditorNormalConverter()
	{
	}

	void EditorNormalConverter::init()
	{
	}

	void EditorNormalConverter::update_UI()
	{
		src_texture_UI_update();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();

		dest_texture_UI_update();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();
		
		copy_texture_UI_update();

		ImGui::Dummy(ImVec2(0.f, 50.f));
		ImGui::Separator();
		if (ImGui::Button("Reset All", ImVec2(0.f, 35.f)))
		{
			reset();
		}
	}

	void EditorNormalConverter::src_texture_UI_update()
	{
		hilight_text("Source Texture");

		{
			std::string curText = "* Current: ";
			if (m_src_texture)
			{
				curText += m_src_texture->get_resource_name();
			}
			else
			{
				curText += "None";
			}
			
			ImGui::Text(curText.c_str());
		}

if (ImGui::Button("Load##Source Texture" , ImVec2(0.f, 25.f)))
{
	std::vector<std::fs::path> vecExt{};
	for (size_t i = 0; i < ::core::name::path::extension::Texture_ArrSize; ++i)
	{
		vecExt.push_back(::core::name::path::extension::Texture[i]);
	}

	const std::fs::path& absTexPath = std::fs::absolute(ResourceManager<Texture>::get_inst().get_base_directory());

	std::fs::path texPath = WinAPI::file_open_dialog(absTexPath, vecExt);

	texPath = PathManager::get_inst().make_path_strkey(texPath);

	m_src_texture = ResourceManager<Texture>::get_inst().load(texPath.string());
}

ImGui::SameLine();

if (ImGui::Button("Clear##Source Texture", ImVec2(0.f, 25.f)))
{
	m_src_texture = nullptr;
}

	}

	void EditorNormalConverter::dest_texture_UI_update()
	{
		using namespace core;
		hilight_text("Dest Texture Save Directory");
		{
			std::string curText = "* Current: ";

			if (m_dest_texture_dir.empty())
			{
				curText += "None";
			}
			else
			{
				curText += m_dest_texture_dir.string();
			}


			ImGui::Text(curText.c_str());
		}


		if (ImGui::Button("Set Directory##Dest Texture", ImVec2(0.f, 25.f)))
		{
			std::vector<std::fs::path> vecExt{};
			for (size_t i = 0; i < ::core::name::path::extension::Texture_ArrSize; ++i)
			{
				vecExt.push_back(::core::name::path::extension::Texture[i]);
			}

			std::fs::path texFile = std::fs::absolute(ResourceManager<Texture>::get_inst().get_base_directory());

			if (m_src_texture)
			{
				texFile /= m_src_texture->get_resource_name();
			}

			m_dest_texture_dir = WinAPI::file_open_dialog(texFile, vecExt);
			m_dest_texture_dir = m_dest_texture_dir.parent_path();
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear##Dest Texture", ImVec2(0.f, 25.f)))
		{
			m_dest_texture_dir.clear();
		}
	}

	void EditorNormalConverter::copy_texture_UI_update()
	{
		using namespace core;
		if (ImGui::Button("Convert Normal Map Texture", ImVec2(0.f, 35.f)))
		{
			if (nullptr == m_src_texture)
			{
				NOTIFICATION("원본 텍스처가 등록되지 않았습니다.");
				return;
			}
			else if (m_dest_texture_dir.empty())
			{
				NOTIFICATION("텍스처파일 출력 경로가 등록되지 않았습니다.");
				return;
			}

			s_ptr<NormalConvertShader> converter = ResourceManager<ComputeShader>::get_inst().load<NormalConvertShader>(::core::name::defaultRes::shader::compute::NormalConvert);

			s_ptr<Texture> convertedTex = converter->convert(m_src_texture);

			if (convertedTex)
			{
				std::fs::path savePath = m_dest_texture_dir;
				std::fs::path texKey = convertedTex->get_resource_name();
				savePath /= texKey.filename();

				if (std::fs::exists(savePath))
				{
					int response = MessageBoxW(nullptr, L"이미 파일이 있습니다. 덮어 쓰시겠습니까?", L"알림", MB_YESNO);

					if (IDNO == response)
					{
						NOTIFICATION("저장을 취소했습니다.");
						return;
					}
				}

				eResult result = ResourceManager<Texture>::get_inst().save_as(convertedTex, savePath.string());
				if (eResult_fail(result))
				{
					NOTIFICATION("변환에 실패했습니다.");
				}
				else
				{
					NOTIFICATION("변환 성공.");
				}
			}
			else
			{
				NOTIFICATION("변환 실패");
			}
		}
	}

	void EditorNormalConverter::reset()
	{
		m_src_texture = nullptr;
		m_dest_texture_dir.clear();
	}
}
