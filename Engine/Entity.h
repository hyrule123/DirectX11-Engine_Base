#pragma once
#include "Engine/Common.h"

//클래스의 헤더 최상단에 작성. 클래스별 이름을 등록한다.
#define CLASS_NAME(_class_) \
public: static inline constexpr const std::string_view concrete_name = #_class_

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_ABLE(_type) \
virtual _type* Clone() override { return new _type(*this); }

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
virtual _type* Clone() override\
{\
	return nullptr;\
}

namespace ehw
{
	class Entity
		: public std::enable_shared_from_this<Entity>
	{
	public:
		Entity(const std::string_view key);
		Entity(const Entity& _other);

		//Clone을 지원하지 않을 경우 nullptr이 반환된다.
		virtual Entity* Clone() { return nullptr; }

		virtual ~Entity();

		inline const std::string_view get_strkey() const { return m_strKey; }
		uint32 GetID() const { return m_ID; }

	private:
		const uint32 m_ID;
		const std::string_view m_strKey;

		static uint32 g_nextID;
	};
}

#include "Engine/Manager/InstanceManager.inl"



