
#include "EditorNormalConverter.h"


#include "../../Manager/PathManager.h"
#include "../../Manager/ResourceManager.h"

#include "../../Util/define_Util.h"

#include "../../Resource/Shader/ComputeShaders/NormalConvertShader.h"

#include "../../DefaultShader/NormalConverter/NormalConverter.hlsli"

namespace editor
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

	void EditorNormalConverter::Init()
	{
	}

	void EditorNormalConverter::UpdateUI()
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
				curText += mTextureSrc->GetStrKey();
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
	for (size_t i = 0; i < ehw::strKey::path::extension::Texture_ArrSize; ++i)
	{
		vecExt.push_back(ehw::strKey::path::extension::Texture[i]);
	}

	const std::fs::path& absTexPath = std::fs::absolute(ehw::ResourceManager<ehw::Texture>::GetBaseDir());

	std::fs::path texPath = ehw::WinAPI::FileDialog(absTexPath, vecExt);

	texPath = ehw::PathManager::MakePathStrKey(texPath);

	mTextureSrc = ehw::ResourceManager<ehw::Texture>::Load(texPath);
}

ImGui::SameLine();

if (ImGui::Button("Clear##Source Texture", ImVec2(0.f, 25.f)))
{
	mTextureSrc = nullptr;
}

	}

	void EditorNormalConverter::DestTextureUpdate()
	{
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
			for (size_t i = 0; i < ehw::strKey::path::extension::Texture_ArrSize; ++i)
			{
				vecExt.push_back(ehw::strKey::path::extension::Texture[i]);
			}

			std::fs::path texFile = std::fs::absolute(ehw::ResourceManager<ehw::Texture>::GetBaseDir());

			if (mTextureSrc)
			{
				texFile /= mTextureSrc->GetStrKey().String();
			}

			mTextureDestDir = ehw::WinAPI::FileDialog(texFile, vecExt);
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

			std::shared_ptr<ehw::NormalConvertShader> converter = ehw::ResourceManager<ehw::iComputeShader>::Load<ehw::NormalConvertShader>(ehw::strKey::defaultRes::shader::compute::NormalConvert);

			std::shared_ptr<ehw::Texture> convertedTex = converter->Convert(mTextureSrc);

			if (convertedTex)
			{
				std::fs::path savePath = mTextureDestDir;
				std::fs::path texKey = convertedTex->GetStrKey().String();
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

				if (eResultFail(convertedTex->Save(savePath)))
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
