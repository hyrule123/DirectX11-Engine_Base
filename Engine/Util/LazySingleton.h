#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

namespace ehw
{
	template<typename T>
	class LazySingleton {
	protected:
		LazySingleton() {}
		~LazySingleton() {}

	private:
		static inline T* m_instance = nullptr;

	public:
		static T& GetInst();
		static void Destroy();
	};

	template<typename T>
	inline T& LazySingleton<T>::GetInst()
	{
		if (nullptr == m_instance)
		{
			m_instance = new T;
		}

		return *(m_instance);
	}

	template<typename T>
	inline void LazySingleton<T>::Destroy() { SAFE_DELETE(m_instance); }
}



