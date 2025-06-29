#pragma once
#include <Engine/Common.h>
#include <Engine/define_Enum.h>

#include <Engine/Game/Entity.h>

#include "Editor/imgui/imgui.h"
#include "Editor/imgui/imgui_stdlib.h"

#include <memory>

namespace core::editor
{
	class EditorEntity
		: public Entity
	{
		CLASS_INFO(EditorEntity, Entity);

	public:
		EditorEntity(const std::string_view _concrete_name);
		virtual void init() override;

		virtual ~EditorEntity();

		void set_save_enable(bool _bSaveEnable) { m_b_enable_save = _bSaveEnable; }
		bool is_save_enable() const { return m_b_enable_save; }

		void set_unique_name(const std::string_view _name) { m_unique_name = _name; }
		const std::string& get_unique_name() const { return m_unique_name; }

	private:
		//데이터를 저장할지 말지 결정(기본 Off)
		bool						m_b_enable_save;

		//저장용으로 사용되는 이름
		std::string m_unique_name;
	};

	void hilight_text(const char* _label, const ImVec2& _size = ImVec2(0.f, 0.f));
}


