#include "Editor/Resource/NormalConverter.h"

#include <Engine/define_Enum.h>

#include "Editor/Widget/Widget_ComboBox.h"

#include <Engine/Manager/PathManager.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/Util/define_Util.h>

#include <Engine/Resource/Shader/ComputeShaders/NormalConvertShader.h>
#include <Engine/DefaultShader/NormalConverter/NormalConverter.hlsli>

namespace ehw::editor
{
	EditorNormalConverter::EditorNormalConverter()
		: EditorWindow("Normal Converter")
		, mTextureSrc{}
		, mTextureDestDir{}
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
		SrcTextureUpdate();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();

		DestTextureUpdate();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();
		
		CopyTextureUpdate();

		ImGui::Dummy(ImVec2(0.f, 50.f));
		ImGui::Separator();
		if (ImGui::Button("Reset All", ImVec2(0.f, 35.f)))
		{
			Reset();
		}
	}

	void EditorNormalConverter::SrcTextureUpdate()
	{
		HilightText("Source Texture");

		{
			std::string curText = "* Current: ";
			if (mTextureSrc)
			{
				curText += mTextureSrc->get_resource_name();
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
	for (size_t i = 0; i < ::ehw::name::path::extension::Texture_ArrSize; ++i)
	{
		vecExt.push_back(::ehw::name::path::extension::Texture[i]);
	}

	const std::fs::path& absTexPath = std::fs::absolute(ResourceManager<Texture>::get_inst().GetBaseDir());

	std::fs::path texPath = WinAPI::FileDialog(absTexPath, vecExt);

	texPath = PathManager::get_inst().MakePathStrKey(texPath);

	mTextureSrc = ResourceManager<Texture>::get_inst().load(texPath.string());
}

ImGui::SameLine();

if (ImGui::Button("Clear##Source Texture", ImVec2(0.f, 25.f)))
{
	mTextureSrc = nullptr;
}

	}

	void EditorNormalConverter::DestTextureUpdate()
	{
		using namespace ehw;
		HilightText("Dest Texture Save Directory");
		{
			std::string curText = "* Current: ";

			if (mTextureDestDir.empty())
			{
				curText += "None";
			}
			else
			{
				curText += mTextureDestDir.string();
			}


			ImGui::Text(curText.c_str());
		}


		if (ImGui::Button("Set Directory##Dest Texture", ImVec2(0.f, 25.f)))
		{
			std::vector<std::fs::path> vecExt{};
			for (size_t i = 0; i < ::ehw::name::path::extension::Texture_ArrSize; ++i)
			{
				vecExt.push_back(::ehw::name::path::extension::Texture[i]);
			}

			std::fs::path texFile = std::fs::absolute(ResourceManager<Texture>::get_inst().GetBaseDir());

			if (mTextureSrc)
			{
				texFile /= mTextureSrc->get_resource_name();
			}

			mTextureDestDir = WinAPI::FileDialog(texFile, vecExt);
			mTextureDestDir = mTextureDestDir.parent_path();
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear##Dest Texture", ImVec2(0.f, 25.f)))
		{
			mTextureDestDir.clear();
		}
	}

	void EditorNormalConverter::CopyTextureUpdate()
	{
		using namespace ehw;
		if (ImGui::Button("Convert Normal Map Texture", ImVec2(0.f, 35.f)))
		{
			if (nullptr == mTextureSrc)
			{
				NOTIFICATION("원본 텍스처가 등록되지 않았습니다.");
				return;
			}
			else if (mTextureDestDir.empty())
			{
				NOTIFICATION("텍스처파일 출력 경로가 등록되지 않았습니다.");
				return;
			}

			std::shared_ptr<NormalConvertShader> converter = ResourceManager<ComputeShader>::get_inst().load<NormalConvertShader>(::ehw::name::defaultRes::shader::compute::NormalConvert);

			std::shared_ptr<Texture> convertedTex = converter->Convert(mTextureSrc);

			if (convertedTex)
			{
				std::fs::path savePath = mTextureDestDir;
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

	void EditorNormalConverter::Reset()
	{
		mTextureSrc = nullptr;
		mTextureDestDir.clear();
	}
}
