#pragma once
#include "Engine/Game/Component/Component.h"

#include "Engine/Common.h"

#include <functional>

namespace core
{
	class Animation;
	class Animator :
		public Component
	{
		CLASS_INFO(Animator, Component);
		

	public:
		struct tEvent
		{
			std::unordered_map<int, std::vector<std::function<void()>>> CallBackFunctions;
		};

	public:
		Animator(const std::string_view key, eDimensionType _type);
		virtual ~Animator() {};

		eDimensionType GetDimensionType() const { return mDimensionType; }

		//아래 함수를 재정의해서 protected된 AddEvent를 호출하는 방식
		virtual bool AddEvent(const std::string_view _animName, uint _frameIdx, const std::function<void()>& _func) { return false; };

		void CallEvent(Animation* _anim, uint _frameIdx);

	protected:
		void AddEvent(Animation* _anim, uint _frameIdx, const std::function<void()>& _func);

	private:
		eDimensionType mDimensionType;

		//애니메이션 포인터 주소
		std::unordered_map<DWORD_PTR, tEvent> mMapEvent;
	};
}


