#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"

template<typename T>
class Singleton {
protected:
	Singleton() {}
	virtual ~Singleton() {}

private:
	static T* m_instance;

public:
	static T& GetInst();
	static void Destroy();
};

template<typename T>
T* Singleton<T>::m_instance = nullptr;

template<typename T>
inline T& Singleton<T>::GetInst()
{
	if (nullptr == m_instance)
	{
		m_instance = new T;
		AtExit::AddFunc(Destroy);
	}

	return *(m_instance);
}

template<typename T>
inline void Singleton<T>::Destroy() { SAFE_DELETE(m_instance); }


