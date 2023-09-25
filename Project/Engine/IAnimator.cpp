#include "PCH_Engine.h"
#include "IAnimator.h"

namespace mh
{
	IAnimator::IAnimator(define::eDimensionType _type)
		: IComponent(define::eComponentType::Animator)
		, mDimensionType(_type)
	{
	}


	void IAnimator::AddEvent(uint _frameIdx, const std::function<void()>& _func)
	{
		auto iter = mMapNotify.find(_frameIdx);
		if (iter == mMapNotify.end())
		{
			mMapNotify.insert(std::make_pair(_frameIdx, std::vector<std::function<void()>>()));
			iter = mMapNotify.find(_frameIdx);
		}
		iter->second.push_back(_func);
	}

}

