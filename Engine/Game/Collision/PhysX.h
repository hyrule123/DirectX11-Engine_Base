#pragma once

#pragma warning(disable : 26495)
#pragma warning(disable : 33010)
#pragma warning(disable : 26812)
#pragma warning(disable : 26451)
#pragma warning(disable : 6297)

//#define PX_PHYSX_STATIC_LIB
#include <PhysX/PxPhysicsAPI.h>

#pragma warning(default : 26495)
#pragma warning(default : 33010)
#pragma warning(default : 26812)
#pragma warning(default : 26451)
#pragma warning(default : 6297)

namespace ehw
{
	//******************* 사용 불가 *******************
	//RefCount는 Physx 내부 용도로 사용하는듯.
	//다른 곳에서 사용시 버그 발생함
	//ex)PxShape에 이걸 사용하면 PxActor 안에 들어가 있는걸로 판단해서 LocalPose 수정이 불가능함
	


	//template <typename T> requires std::is_base_of_v<physx::PxRefCounted, T>
	//class PhysXSharedPtr
	//{
	//public:
	//	PhysXSharedPtr() : m_ptr(nullptr) {};
	//	PhysXSharedPtr(T* _ptr) : m_ptr(_ptr) { InternalAddRef(); }
	//	PhysXSharedPtr(const PhysXSharedPtr& _other) : m_ptr(_other.m_ptr) { InternalAddRef(); }
	//	PhysXSharedPtr(PhysXSharedPtr&& _move) noexcept :m_ptr(nullptr) { Swap(_move); }
	//	~PhysXSharedPtr() { InternalRelease(); };

	//	void operator = (const PhysXSharedPtr& _other)
	//	{
	//		if (this != &_other)
	//		{
	//			PhysXSharedPtr temp(m_ptr);
	//			m_ptr = _other.m_ptr;
	//			InternalAddRef();
	//		}
	//	}

	//	void operator = (PhysXSharedPtr&& _move) noexcept { if (this != &_move) { Swap(_move); } }

	//	inline T* Get() { return m_ptr; }
	//	inline T* Get() const { return m_ptr; }
	//	inline T* operator->() { return m_ptr; }
	//	inline T* operator->() const { return m_ptr; }

	//	inline T& operator*() { return *m_ptr; }
	//	inline const T& operator*() const { return *m_ptr; }
	//	

	//	inline operator bool() const { return (nullptr != m_ptr); }

	//private:
	//	void InternalAddRef() { if (m_ptr) { m_ptr->acquireReference(); } }
	//	void InternalRelease() { PX_RELEASE(m_ptr); }

	//	void Swap(PhysXSharedPtr& _other) { T* temp = m_ptr; m_ptr = _other.m_ptr; _other.m_ptr = temp; }

	//private:
	//	T* m_ptr;
	//};
}
