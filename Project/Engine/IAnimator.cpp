#include "PCH_Engine.h"
#include "IAnimator.h"

namespace mh
{
	IAnimator::IAnimator(define::eDimensionType _type)
		: IComponent(define::eComponentType::Animator)
		, mDimensionType(_type)
	{
	}


	void IAnimator::AddEvent(IAnimation* _anim, int _frameIdx, const std::function<void()>& _func)
	{
		MH_ASSERT(nullptr != _anim || 0 < _frameIdx || _func);

		using keyType = decltype(mMapEvent)::key_type;
		
		UINT64 key = reinterpret_cast<keyType>(_anim);

		//해당 애니메이션에 대한 이벤트가 만들어져있지 않을 경우 이벤트를 추가
		auto eventIter = mMapEvent.find(key);
		if (eventIter == mMapEvent.end())
		{
			mMapEvent.insert(std::make_pair(key, IAnimator::tEvent()));
			eventIter = mMapEvent.find(key);
		}
		
		//만든 이벤트에 함수 추가
		auto callbackFuncIter = eventIter->second.CallBackFunctions.find(_frameIdx);
		if (callbackFuncIter == eventIter->second.CallBackFunctions.end())
		{

		}
	}

}

