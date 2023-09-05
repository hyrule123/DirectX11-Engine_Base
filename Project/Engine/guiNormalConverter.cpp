#include "PCH_Engine.h"
#include "guiNormalConverter.h"

#include "PathMgr.h"
#include "define_Util.h"

namespace gui
{
	guiNormalConverter::guiNormalConverter()
		: guiWindow("Normal Converter")
	{
	}
	guiNormalConverter::~guiNormalConverter()
	{
	}

	void guiNormalConverter::Init()
	{
	}

	void guiNormalConverter::UpdateUI()
	{
		SrcTextureUpdate();
	}

	void guiNormalConverter::SrcTextureUpdate()
	{
		if (ImGui::Button("Load Source Texture", ImVec2(0.f, 35.f)))
		{
			std::vector<std::fs::path> vecExt{};
			for (size_t i = 0; i < mh::define::strKey::Ext_Tex_Size; ++i)
			{
				vecExt.push_back(mh::define::strKey::Ext_Tex[i]);
			}
			
			std::fs::path texPath = mh::WinAPI::FileDialog(mh::PathMgr::GetContentPathAbsolute(mh::eResourceType::Texture), vecExt);
			
			texPath = mh::PathMgr::MakePathStrKey(texPath);
			
		}

	}

}
