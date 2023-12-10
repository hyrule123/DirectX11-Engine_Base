#include "PCH_Engine.h"
#include "guiEntity.h"
#include "json-cpp/json.h"

namespace editor
{
	UINT32 guiEntity::g_NextID{};

	guiEntity::guiEntity(const std::string_view _strName)
		: m_ID(++g_NextID)
		, m_Name(_strName)
		, m_StrKey(_strName)
		, m_bSaveEnable()	
	{
	}


	guiEntity::~guiEntity()
	{
	}
	
}


