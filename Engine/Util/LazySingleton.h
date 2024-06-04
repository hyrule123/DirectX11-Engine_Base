#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

template<typename T>
class LazySingleton {
protected:
	LazySingleton() {}
	virtual ~LazySingleton() {}

private:
	static T* m_instance;

public:
	static T& GetInst();
	static void Destroy();
};

template<typename T>
T* LazySingleton<T>::m_instance = nullptr;

template<typename T>
inline T& LazySingleton<T>::GetInst()
{
	if (nullptr == m_instance)
	{
		m_instance = new T;
		AtExit::AddFunc(Destroy);
	}

	return *(m_instance);
}

template<typename T>
inline void LazySingleton<T>::Destroy() { SAFE_DELETE(m_instance); }


