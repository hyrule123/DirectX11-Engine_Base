#pragma once
#include <Engine/Common.h>
#include <Engine/define_Enum.h>

#include "Editor/imgui/imgui.h"
#include "Editor/imgui/imgui_stdlib.h"

#include <memory>

namespace ehw::editor
{
	class EditorEntity
		: public std::enable_shared_from_this<EditorEntity>
	{
	public:
		EditorEntity(const std::string_view _strName);
		virtual ~EditorEntity();

		UINT32 GetID() const { return m_ID; }

		const std::string& GetName() const { return m_Name; }

		void MakeUniqueKeyByName() { m_StrKey += "##"; m_StrKey += std::to_string(m_ID); }
		void SetStrKey(const std::string_view _strKey) { m_StrKey = _strKey; }
		const std::string& get_path_key() const { return m_StrKey; }
		
		void SetSaveEnable(bool _bSaveEnable) { m_bSaveEnable = _bSaveEnable; }
		bool IsSaveEnable() const { return m_bSaveEnable; }

	private:
		static UINT32				g_NextID;
		const UINT32				m_ID;

		//이름(중복 가능)
		std::string					m_Name;
		
		//Key(중복 불가)
		std::string					m_StrKey;

		//데이터를 저장할지 말지 결정(기본 Off)
		bool						m_bSaveEnable;
	};

	void HilightText(const char* _label, const ImVec2& _size = ImVec2(0.f, 0.f));
}


