#include "PCH_Editor.h"
#include "Base/EditorEntity.h"


namespace editor
{
	UINT32 EditorEntity::g_NextID{};

	EditorEntity::EditorEntity(const std::string_view _strName)
		: m_ID(++g_NextID)
		, m_Name(_strName)
		, m_StrKey(_strName)
		, m_bSaveEnable()	
	{
	}


	EditorEntity::~EditorEntity()
	{
	}
	
}


