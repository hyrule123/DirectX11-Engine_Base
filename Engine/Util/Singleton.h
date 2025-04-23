#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

#define SINGLETON_ONLY(T) friend class Singleton<T>; private: T(); ~T(); T(const T& _copy) = delete; T(T&& _move) = delete

namespace core
{
	template<typename T>
	class Singleton {
	protected:
		Singleton() {}
		~Singleton() {}

	private:
		static inline T* m_instance = nullptr;

	public:
		static T& get_inst() {
			if (nullptr == m_instance)
			{
				m_instance = new T;
			}
			return *(m_instance);
		}
		static void Destroy() { SAFE_DELETE(m_instance); }
	};
}



