#pragma once
#include "IComponent.h"
#include "define_Struct.h"


namespace mh
{
	class IAnimator :
		public IComponent
	{
	public:
		struct tEvent
		{
			std::function<void()> CallbackFunc;
			bool bCalled;
		};

		IAnimator(define::eDimensionType _type);
		virtual ~IAnimator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;

		virtual bool IsPlaying() const = 0;

		define::eDimensionType GetDimensionType() const { return mDimensionType; }

		void AddNotify(uint _frameIdx, const std::function<void()>& _func);

	private:
		define::eDimensionType mDimensionType;

		std::unordered_map<uint, std::function<void()>, tHashFuncFast_UINT32, std::equal_to<>> mMapNotify;
	};
}


