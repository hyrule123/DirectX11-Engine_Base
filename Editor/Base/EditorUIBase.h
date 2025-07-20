#pragma once
#include <Editor/Base/EditorEntity.h>
#include <Editor/strKey_Editor.h>

#include <Engine/Util/Serialize/Serializable.h>

namespace core::editor
{
	class EditorUIBase 
		: public EditorEntity
		, public Serializable_Json
	{
		CLASS_INFO(EditorUIBase, EditorEntity);
	public:
		EditorUIBase(const std::string_view _strName);
		//virtual void init() override;
		virtual ~EditorUIBase();

		void final_update();

		virtual void update() {}
		
		//true 반환 시 update_UI와 end_UI를 호출한다.
		virtual bool begin_UI() = 0;

		//여기에는 클래스에서 특별하게 호출해야할 레이아웃이 있을 경우 재정의해서 사용하면 됨.
		virtual void update_UI() {};

		/*
		* 주의 - 재정의할 경우 Super::end_UI를 가장 마지막에 호출해야함.
		* 재정의해서 각자 guiBase에 맞는 end함수를 호출
		*/
		virtual void end_UI() {};

		virtual eResult serialize_json(JsonSerializer* _ser) const override;
		virtual eResult deserialize_json(const JsonSerializer* _ser) override;

		bool is_enabled() { return m_b_enable; }

		//ImGui API에 필요
		bool* is_enabled_ptr() { return &m_b_enable; }

		void set_enable(bool _bEnable) { m_b_enable = _bEnable; }
		void close() { m_b_enable = false; }

	private:
		bool						m_b_enable;
	};
}



