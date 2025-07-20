#include "EditorUIBase.h"
#include <Editor/EditorManager.h>

#include <Engine/Define_Macro.h>
#include <Engine/Util/Serialize/JsonSerializer.h>


namespace core::editor
{
	EditorUIBase::EditorUIBase(const std::string_view _strName)
		: EditorEntity(_strName)
	{
	}


	EditorUIBase::~EditorUIBase()
	{
	}

	void EditorUIBase::final_update()
	{
		//자신의 고유 update 항목을 호출
		update();
		
		if (begin_UI())
		{
			update_UI();
			end_UI();
		}
	}

	eResult EditorUIBase::serialize_json(JsonSerializer* _ser) const
	{
		SERIALIZER_CHECK_PTR(_ser);

		JsonSerializer& ser = *_ser;
		
		JSON_TRY
		{
			ser[JSON_KEY(m_b_enable)] << m_b_enable;
		}
		JSON_CATCH;

		return eResult::Success;
	}

	eResult EditorUIBase::deserialize_json(const JsonSerializer* _ser)
	{
		SERIALIZER_CHECK_PTR(_ser);

		const JsonSerializer& ser = *_ser;

		JSON_TRY
		{
			ser[JSON_KEY(m_b_enable)] >> m_b_enable;
		}
		JSON_CATCH;

		return eResult::Success;
	}
}
