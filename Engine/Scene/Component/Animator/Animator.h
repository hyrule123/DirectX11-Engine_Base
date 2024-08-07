#pragma once
#include "Engine/Scene/Component/Component.h"

#include "Engine/Common.h"

#include <functional>

namespace ehw
{
	class Animation;
	class Animator :
		public Component<Animator, eComponentCategory::Animator>
	{
	public:
		struct tEvent
		{
			std::unordered_map<int, std::vector<std::function<void()>>> CallBackFunctions;
		};

	public:
		Animator(eDimensionType _type);
		virtual ~Animator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;

		virtual bool IsPlaying() const = 0;

		eDimensionType GetDimensionType() const { return mDimensionType; }

		//아래 함수를 재정의해서 protected된 AddEvent를 호출하는 방식
		virtual bool AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func) { return false; };

		void CallEvent(Animation* _anim, int _frameIdx);

	protected:
		void AddEvent(Animation* _anim, int _frameIdx, const std::function<void()>& _func);

	private:
		eDimensionType mDimensionType;

		//애니메이션 포인터 주소
		std::unordered_map<DWORD_PTR, tEvent> mMapEvent;
	};
}


