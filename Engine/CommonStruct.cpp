#include "Engine/CommonStruct.h"

namespace ehw
{
	union_ColliderID::union_ColliderID(uint32 _1, uint32 _2)
		: Low32()
		, High32()
	{
		if (_1 <= _2)
		{
			Low32 = _1;
			High32 = _2;
		}
		else
		{
			Low32 = _2;
			High32 = _1;
		}
	}
}
