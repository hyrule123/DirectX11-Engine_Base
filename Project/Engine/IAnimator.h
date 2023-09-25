#pragma once
#include "IComponent.h"
#include "define_Struct.h"


namespace mh
{
	class IAnimator :
		public IComponent
	{
	public:
		IAnimator(define::eDimensionType _type);
		virtual ~IAnimator() {};

		virtual void BindData() = 0;
		virtual void UnBindData() = 0;

		virtual bool IsPlaying() const = 0;

		define::eDimensionType GetDimensionType() const { return mDimensionType; }

		void AddEvent(uint _frameIdx, const std::function<void()>& _func);

	protected:
		inline void CallEvent(uint _frameIdx);

	private:
		define::eDimensionType mDimensionType;

		std::unordered_map<uint, std::vector<std::function<void()>>, tHashFuncFast_UINT32, std::equal_to<>> mMapNotify;
	};

	inline void IAnimator::CallEvent(uint _frameIdx)
	{
		const auto& iter = mMapNotify.find(_frameIdx);
		if (iter != mMapNotify.end())
		{
			for (size_t i = 0; i < iter->second.size(); ++i)
			{
				iter->second[i]();
			}
		}
	}
}


