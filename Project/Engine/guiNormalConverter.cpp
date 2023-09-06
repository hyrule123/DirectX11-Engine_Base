#include "PCH_Engine.h"
#include "guiNormalConverter.h"

#include "PathMgr.h"
#include "ResMgr.h"
#include "define_Util.h"

#include "NormalConvertShader.h"
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
		
		mComboDestR	.SetKey("SrcX");
		mComboDestG	.SetKey("SrcY");
		mComboDestB	.SetKey("SrcZ");


		//몬헌 Normal이 Y = -Normal이고
		//프로젝트에서는 Z = Normal 이므로
		mComboDestR.SetItems(XYZ);
		mComboDestR.SetCurrentIndex((int)eXYZSign::X);

		mComboDestG.SetItems(XYZ);
		mComboDestG.SetCurrentIndex((int)eXYZSign::MinusZ);

		mComboDestB.SetItems(XYZ);
		mComboDestB.SetCurrentIndex((int)eXYZSign::Y);
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

		ImGui::Dummy(ImVec2(0.f, 50.f));
		ImGui::Separator();
		if (ImGui::Button("Reset All", ImVec2(0.f, 35.f)))
		{
			Reset();
		}
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

		mComboDestR.FixedUpdate();
		mComboDestG.FixedUpdate();
		mComboDestB.FixedUpdate();
	}

	void guiNormalConverter::CopyTextureUpdate()
	{
		if (ImGui::Button("Convert Normal Map Texture", ImVec2(0.f, 35.f)))
		{
			if (nullptr == mTextureSrc)
			{
				NOTIFICATION_W(L"원본 텍스처가 등록되지 않았습니다.");
				return;
			}
			else if (mTextureDestPath.empty())
			{
				NOTIFICATION_W(L"텍스처파일 출력 경로가 등록되지 않았습니다.");
				return;
			}

			std::shared_ptr<mh::NormalConvertShader> converter =  mh::ResMgr::Load<mh::NormalConvertShader>(mh::strKey::Default::shader::compute::NormalConvert);

			mh::NormalConvertShader::Desc desc{};
			
			guiNormalConverter::AxisAndSign srcR = GetXYZSignToHLSLFormat((eXYZSign)mComboDestR.GetCurrentIndex());
			desc.DestAxis.x = (float)srcR.Axis;
			desc.DestSign.x = (float)srcR.Sign;

			guiNormalConverter::AxisAndSign srcG = GetXYZSignToHLSLFormat((eXYZSign)mComboDestG.GetCurrentIndex());
			desc.DestAxis.y = (float)srcG.Axis;
			desc.DestSign.y = (float)srcG.Sign;

			guiNormalConverter::AxisAndSign srcB = GetXYZSignToHLSLFormat((eXYZSign)mComboDestB.GetCurrentIndex());
			desc.DestAxis.z = (float)srcB.Axis;
			desc.DestSign.z = (float)srcB.Sign;
			
			desc.SrcTex = mTextureSrc;

			std::shared_ptr<mh::Texture> convertedTex = converter->Convert(desc);

			if (convertedTex)
			{
				convertedTex->Save(mTextureDestPath);
			}
			else
			{
				NOTIFICATION_W(L"변환 실패");
			}
		}
	}

	void guiNormalConverter::Reset()
	{
		mTextureSrc = nullptr;
		mTextureDestPath.clear();
		mCB = {};

		mComboDestR.SetCurrentIndex((int)eXYZSign::X);
		mComboDestR.SetCurrentIndex((int)eXYZSign::MinusZ);
		mComboDestR.SetCurrentIndex((int)eXYZSign::Y);
	}
}
