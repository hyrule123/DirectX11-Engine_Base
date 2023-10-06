#pragma once
#include "IComponent.h"
#include "define_Struct.h"


namespace mh
{
	class IAnimation;
	class IAnimator :
		public IComponent
	{
	public:
		struct tEvent
		{
			std::unordered_map<int, std::vector<std::function<void()>>, tFastHashFunc<int>> CallBackFunctions;
		};

	public:
		IAnimator(define::eDimensionType _type);
		virtual ~IAnimator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;

		virtual bool IsPlaying() const = 0;

		define::eDimensionType GetDimensionType() const { return mDimensionType; }

		void AddEvent(IAnimation* _anim, int _frameIdx, const std::function<void()>& _func);

	private:
		define::eDimensionType mDimensionType;

		//Key: 애니메이션의 포인터 주소
		std::unordered_map<UINT64, tEvent, tFastHashFunc<UINT64>> mMapEvent;
	};
}


