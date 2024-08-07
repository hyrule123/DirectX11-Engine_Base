#pragma once
#include "Engine/Common.h"

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_ABLE(_type) \
virtual _type* Clone() override { return new _type(*this); }
//virtual bool IsCloneable() const override { return true; }

//Clone을 지원하지 않을 경우 nullptr이 반환된다.
#define CLONE_DISABLE(_type) \
virtual _type* Clone() override\
{\
	return nullptr;\
}
//virtual bool IsCloneable() const override { return false; }

namespace ehw
{
	namespace strKey::Json::Entity
	{
		STRKEY_DECLARE(m_strKey);
	}

	class Entity 
	{
		
	public:
		Entity();

		Entity(const Entity& _other);

		//Clone을 지원하지 않을 경우 nullptr이 반환된다.
		virtual Entity* Clone() { return nullptr; }

		Entity(Entity&& _move) noexcept;

		virtual ~Entity();

		inline void SetStrKey(const std::string_view _strKey) { m_strKey = _strKey; }
		inline const std::string& GetStrKey() const { return m_strKey; }
		uint32 GetID() const { return m_ID; }

	protected:
		void Swap(Entity& _other);


	private:
		const uint32 m_ID;
		std::string m_strKey;

		static uint32 g_nextID;
	};


}






