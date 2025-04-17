#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

namespace core
{
	template<typename T>
	class LazySingleton {
	protected:
		LazySingleton() {}
		~LazySingleton() {}

	private:
		static inline T* m_instance = nullptr;

	public:
		static T& get_inst();
		static void Destroy();
	};

	template<typename T>
	inline T& LazySingleton<T>::get_inst()
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



