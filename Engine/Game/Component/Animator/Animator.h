#pragma once
#include <Engine/Game/Component/Component.h>

#include <Engine/Common.h>

#include <functional>

namespace core
{
	class Animation;
	class Animator :
		public Component
	{
		CLASS_INFO(Animator, Component);
		BASE_COMPONENT(eComponentOrder::Animator);

	public:
		struct tEvent
		{
			std::unordered_map<int, std::vector<std::function<void()>>> callback_functions;
		};

	public:
		Animator(eDimensionType _type);
		virtual ~Animator() {};

		eDimensionType get_dimension_type() const { return m_dimension_type; }

		//아래 함수를 재정의해서 protected된 AddEvent를 호출하는 방식
		virtual bool add_event(const std::string_view _animName, uint _frameIdx, const std::function<void()>& _func) { return false; };

		void CallEvent(Animation* _anim, uint _frameIdx);

	protected:
		void add_event(Animation* _anim, uint _frameIdx, const std::function<void()>& _func);

	private:
		eDimensionType m_dimension_type;

		//애니메이션 포인터 주소
		std::unordered_map<DWORD_PTR, tEvent> m_map_event;
	};
}


