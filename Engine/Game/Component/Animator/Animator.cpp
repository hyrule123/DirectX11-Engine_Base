
#include "Animator.h"

namespace core
{
	Animator::Animator(eDimensionType _type)
		: Super(s_component_order)
		, m_dimension_type(_type)
	{
	}

	void Animator::add_event(Animation* _anim, uint _frameIdx, const std::function<void()>& _func)
	{
		ASSERT(nullptr != _anim || 0 < _frameIdx || _func, "애니메이션이 없거나 ");

		using keyType = decltype(m_map_event)::key_type;
		
		keyType key = reinterpret_cast<keyType>(_anim);

		//해당 애니메이션에 대한 이벤트가 만들어져있지 않을 경우 이벤트를 추가
		auto eventIter = m_map_event.find(key);
		if (eventIter == m_map_event.end())
		{
			m_map_event.insert(std::make_pair(key, Animator::tEvent()));
			eventIter = m_map_event.find(key);
		}
		
		//해당 프레임에 생성되어있는 이벤트(vector)가 있는지 확인.
		//없을 경우 이벤트를 추가
		auto callbackFuncIter = eventIter->second.callback_functions.find(_frameIdx);
		if (callbackFuncIter == eventIter->second.callback_functions.end())
		{
			auto pair = eventIter->second.callback_functions.insert(
				std::make_pair(_frameIdx, std::vector<std::function<void()>>()));
			callbackFuncIter = eventIter->second.callback_functions.find(_frameIdx);
		}

		callbackFuncIter->second.push_back(_func);
	}

	void Animator::CallEvent(Animation* _anim, uint _frameIdx)
	{
		//해당 애니메이션 포인터 주소로 등록된 이벤트가 있는지 확인
		auto eventIter = m_map_event.find(reinterpret_cast<DWORD_PTR>(_anim));
		if (eventIter == m_map_event.end())
		{
			return;
		}

		//있을 경우 해당 애니메이션의 프레임 번호에 등록된 이벤트를 확인
		auto callbackFuncIter = eventIter->second.callback_functions.find(_frameIdx);
		if (callbackFuncIter == eventIter->second.callback_functions.end())
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

