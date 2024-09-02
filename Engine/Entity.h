#pragma once
#include "Engine/Common.h"

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_ABLE(_type) \
virtual _type* Clone() override { return new _type(*this); }

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
virtual _type* Clone() override\
{\
	return nullptr;\
}
#define CLASS_INFO(_name) namespace class_name { STR_KEY(_name); }

namespace ehw
{
	CLASS_INFO(Entity);
	class Entity
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






