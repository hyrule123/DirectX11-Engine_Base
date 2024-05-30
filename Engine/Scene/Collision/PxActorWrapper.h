#pragma once
#include <type_traits>

#include <PhysX/PxPhysicsAPI.h>


namespace ehw
{
	template <class T> requires std::derived_from<T, physx::PxActor>
	class PxActorWrapper
	{
	public:
		PxActorWrapper();
		~PxActorWrapper();
		
		void Reset();

		void Set(T* const _pxActor);
		T* Get() { return m_pxActor; }
		const T* Get() const { return m_pxActor; }
		T* operator->() { return m_pxActor; }
		const T* operator->() const { return m_pxActor; }

		explicit operator bool() const { return nullptr != m_pxActor; }
		

		template <typename U> requires std::derived_from<T, physx::PxActor>
		U* is() { U* ret{}; if (m_pxActor) { ret = m_pxActor->is<U>(); } return ret; }

	private:
		T* m_pxActor;
	};

	template <class T> requires std::derived_from<T, physx::PxActor>
	inline PxActorWrapper<T>::PxActorWrapper()
		: m_pxActor{ nullptr }
	{
	}

	template <class T> requires std::derived_from<T, physx::PxActor>
	inline PxActorWrapper<T>::~PxActorWrapper()
	{
		Reset();
	}

	template <class T> requires std::derived_from<T, physx::PxActor>
	inline void PxActorWrapper<T>::Reset()
	{
		if (m_pxActor)
		{
			physx::PxScene* scene = m_pxActor->getScene();
			if (scene)
			{
				scene->removeActor(*static_cast<physx::PxActor*>(m_pxActor));
				m_pxActor->release();
				m_pxActor = nullptr;
			}
		}
	}

	template <class T> requires std::derived_from<T, physx::PxActor>
	inline void PxActorWrapper<T>::Set(T* const _pxActor)
	{
		Reset();
		m_pxActor = _pxActor;
	}
}
