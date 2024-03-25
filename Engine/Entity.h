#pragma once
#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"
#include "Engine/CommonType.h"

#include <memory>

#define CLONE_ABLE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }

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
		virtual Entity* Clone() { ASSERT(false, "Entity 클래스는 Clone할수 없습니다."); return nullptr; }

		Entity(Entity&& _move) noexcept;

		virtual ~Entity();

		inline void SetStrKey(const std::string_view _strKey) { m_strKey = _strKey; }
		inline const std::string& GetStrKey() const { return m_strKey; }
		UINT32 GetID() const { return mID; }



	private:
		std::string m_strKey;
		const UINT32 mID;

		static UINT32 gIDNext;
	};


}






