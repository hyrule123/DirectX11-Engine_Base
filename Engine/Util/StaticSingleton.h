#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

namespace ehw {
	template <class T>
	class StaticSingleton
	{
	public:
		static inline T& GetInst() noexcept { return m_inst; }
	protected:
		StaticSingleton() {};
		~StaticSingleton() {};

	private:
		static T m_inst;
	};

	template <class T>
	T StaticSingleton<T>::m_inst{};
}


