#include "Engine/Game/Component/Animator/iAnimator.h"

namespace ehw
{
	iAnimator::iAnimator(eDimensionType _type)
		: mDimensionType(_type)
	{
	}


	void iAnimator::AddEvent(iAnimation* _anim, int _frameIdx, const std::function<void()>& _func)
	{
		ASSERT(nullptr != _anim || 0 < _frameIdx || _func, "애니메이션이 없거나 ");

		using keyType = decltype(mMapEvent)::key_type;
		
		keyType key = reinterpret_cast<keyType>(_anim);

		//해당 애니메이션에 대한 이벤트가 만들어져있지 않을 경우 이벤트를 추가
		auto eventIter = mMapEvent.find(key);
		if (eventIter == mMapEvent.end())
		{
			mMapEvent.insert(std::make_pair(key, iAnimator::tEvent()));
			eventIter = mMapEvent.find(key);
		}
		
		//해당 프레임에 생성되어있는 이벤트(vector)가 있는지 확인.
		//없을 경우 이벤트를 추가
		auto callbackFuncIter = eventIter->second.CallBackFunctions.find(_frameIdx);
		if (callbackFuncIter == eventIter->second.CallBackFunctions.end())
		{
			auto pair = eventIter->second.CallBackFunctions.insert(
				std::make_pair(_frameIdx, std::vector<std::function<void()>>()));
			callbackFuncIter = eventIter->second.CallBackFunctions.find(_frameIdx);
		}

		callbackFuncIter->second.push_back(_func);
	}

	void iAnimator::CallEvent(iAnimation* _anim, int _frameIdx)
	{
		//해당 애니메이션 포인터 주소로 등록된 이벤트가 있는지 확인
		auto eventIter = mMapEvent.find(reinterpret_cast<DWORD_PTR>(_anim));
		if (eventIter == mMapEvent.end())
		{
			return;
		}

		//있을 경우 해당 애니메이션의 프레임 번호에 등록된 이벤트를 확인
		auto callbackFuncIter = eventIter->second.CallBackFunctions.find(_frameIdx);
		if (callbackFuncIter == eventIter->second.CallBackFunctions.end())
		{
			return;
		}

		//이벤트가 있을 경우 순회돌면서 호출
		for (size_t i = 0; i < callbackFuncIter->second.size(); ++i)
		{
			callbackFuncIter->second[i]();
		}
	}
}

