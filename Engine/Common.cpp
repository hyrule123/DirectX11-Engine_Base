
#include "Common.h"

namespace core
{
	ColliderID::ColliderID(uint32 _1, uint32 _2)
	{
		if (_1 <= _2)
		{
			m_ID.ID32_pair.lower = _1;
			m_ID.ID32_pair.higher = _2;
		}
		else
		{
			m_ID.ID32_pair.lower = _2;
			m_ID.ID32_pair.higher = _1;
		}
	}
	ColliderID::~ColliderID()
	{
	}
}
