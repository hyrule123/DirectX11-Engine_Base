#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

namespace ehw {
	template <class T>
	class StaticSingleton
	{
	public:
		static inline T& GetInst() noexcept {
			static T inst{};
			return inst; 
		}
	protected:
		StaticSingleton() {};
		~StaticSingleton() {};

		StaticSingleton(const StaticSingleton&) = delete;
		StaticSingleton& operator=(const StaticSingleton&) = delete;
	};
}


