#include "PCH_Engine.h"
#include "guiNormalConverter.h"

#include "PathMgr.h"
#include "ResMgr.h"
#include "define_Util.h"
#include "NormalConverter.h"
#include "DefaultShader/SH_NormalConvert.hlsli"

namespace gui
{
	guiNormalConverter::guiNormalConverter()
		: guiWindow("Normal Converter")
		, mTextureSrc{}
		, mTextureDestPath{}
		, mCB{}
	{
	}
	guiNormalConverter::~guiNormalConverter()
	{
	}

	void guiNormalConverter::Init()
	{
		std::vector<std::string> XYZ((int)eXYZSign::XYZSignEnd);
		XYZ[(int)eXYZSign::X] = "X";
		XYZ[(int)eXYZSign::Y] = "Y";
		XYZ[(int)eXYZSign::Z] = "Z";

		XYZ[(int)eXYZSign::MinusX] = "-X";
		XYZ[(int)eXYZSign::MinusY] = "-Y";
		XYZ[(int)eXYZSign::MinusZ] = "-Z";
		
		mComboSrcR	.SetKey("SrcX");
		mComboSrcG	.SetKey("SrcY");
		mComboSrcB	.SetKey("SrcZ");

		mComboSrcR.SetItems(XYZ);
		mComboSrcR.SetCurrentIndex((int)eXYZSign::X);

		mComboSrcG.SetItems(XYZ);
		mComboSrcR.SetCurrentIndex((int)eXYZSign::MinusZ);

		mComboSrcB.SetItems(XYZ);
		mComboSrcR.SetCurrentIndex((int)eXYZSign::Y);
	}

	void guiNormalConverter::UpdateUI()
	{
		SrcTextureUpdate();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();

		DestTextureUpdate();

		ImGui::Dummy(ImVec2(0.f, 15.f));
		ImGui::Separator();
		
		CopyTextureUpdate();
	}

	void guiNormalConverter::SrcTextureUpdate()
	{
		HilightText("Source Texture");

		{
			std::string curText = "* Current: ";
			if (mTextureSrc)
			{
				curText += mTextureSrc->GetKey();
			}
			else
			{
				curText += "None";
			}
			
			ImGui::Text(curText.c_str());
		}

		if (ImGui::Button("Load##Source Texture"))
		{
			std::vector<std::fs::path> vecExt{};
			for (size_t i = 0; i < mh::define::strKey::Ext_Tex_Size; ++i)
			{
				vecExt.push_back(mh::define::strKey::Ext_Tex[i]);
			}
			
			std::fs::path texPath = mh::WinAPI::FileDialog(mh::PathMgr::GetContentPathAbsolute(mh::eResourceType::Texture), vecExt);
			
			texPath = mh::PathMgr::MakePathStrKey(texPath);
			
			mTextureSrc = mh::ResMgr::Load<mh::Texture>(texPath);
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear##Source Texture"))
		{
			mTextureSrc = nullptr;
		}

	}

	void guiNormalConverter::DestTextureUpdate()
	{
		HilightText("Dest Texture Save Path");
		{
			std::string curText = "* Current: ";

			if (mTextureDestPath.empty())
			{
				curText += "None";
			}
			else
			{
				curText += mTextureDestPath.string();
			}


			ImGui::Text(curText.c_str());
		}


		if (ImGui::Button("Load##Dest Texture"))
		{
			std::vector<std::fs::path> vecExt{};
			for (size_t i = 0; i < mh::define::strKey::Ext_Tex_Size; ++i)
			{
				vecExt.push_back(mh::define::strKey::Ext_Tex[i]);
			}

			mTextureDestPath = mh::WinAPI::FileDialog(mh::PathMgr::GetContentPathAbsolute(mh::eResourceType::Texture), vecExt);

			if (false == mTextureDestPath.has_extension())
			{
				NOTIFICATION_W(L"확장자를 입력 해주세요.");
				mTextureDestPath.clear();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Clear##Dest Texture"))
		{
			mTextureDestPath.clear();
		}
	}

	void guiNormalConverter::CopyTextureUpdate()
	{
		if (ImGui::Button("Convert Normal Map Texture", ImVec2(0.f, 35.f)))
		{
			if (nullptr == mTextureSrc)
			{
				NOTIFICATION_W(L"원본 텍스처가 등록되지 않았습니다.");
			}
			else if (mTextureDestPath.empty())
			{
				NOTIFICATION_W(L"텍스처 타깃 경로가 등록되지 않았습니다.");
			}





		}
	}








}
