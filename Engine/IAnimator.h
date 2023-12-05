#pragma once
#include "iComponent.h"
#include "define_Struct.h"


namespace ehw
{
	class iAnimation;
	class iAnimator :
		public iComponent
	{
	public:
		struct tEvent
		{
			std::unordered_map<int, std::vector<std::function<void()>>, tFastHashFunc<int>> CallBackFunctions;
		};

	public:
		iAnimator(eDimensionType _type);
		virtual ~iAnimator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;

		virtual bool IsPlaying() const = 0;

		eDimensionType GetDimensionType() const { return mDimensionType; }

		//아래 함수를 재정의해서 protected된 AddEvent를 호출하는 방식
		virtual bool AddEvent(const std::string_view _animName, int _frameIdx, const std::function<void()>& _func) { return false; };

		void CallEvent(iAnimation* _anim, int _frameIdx);

	protected:
		void AddEvent(iAnimation* _anim, int _frameIdx, const std::function<void()>& _func);

	private:
		eDimensionType mDimensionType;

		//애니메이션 포인터 주소
		std::unordered_map<DWORD_PTR, tEvent, tFastHashFunc<DWORD_PTR>> mMapEvent;
	};
}


