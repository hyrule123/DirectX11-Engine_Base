#include "Engine/Common.h"

namespace core
{
	tColliderID::tColliderID(uint32 _1, uint32 _2)
		: ID64()
	{
		if (_1 <= _2)
		{
			ID64.pair.Low32 = _1;
			ID64.pair.High32 = _2;
		}
		else
		{
			ID64.pair.Low32 = _2;
			ID64.pair.High32 = _1;
		}
	}
	tColliderID::~tColliderID()
	{
	}
}
