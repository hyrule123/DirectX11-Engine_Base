#pragma once
#include "define_Enum.h"
#include "define_Macro.h"
#include "CommonType.h"

#include <memory>

#define CLONE_ABLE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }

namespace ehw
{
	namespace strKey::Json::Entity
	{
		STRKEY_DECLARE(mStrKey);
	}

	class Entity 
		: public std::enable_shared_from_this<Entity>
	{
	public:
		Entity();

		Entity(const Entity& _other);
		virtual Entity* Clone() { ASSERT(false, "Entity 클래스는 Clone할수 없습니다."); return nullptr; }

		Entity(Entity&& _move) noexcept;

		virtual ~Entity();

		void SetStrKey(const std::string_view _strKey) { mStrKey = _strKey; }
		const std::string& GetStrKey() const { return mStrKey; }
		UINT32 GetID() const { return mID; }

		template <typename T>
		inline std::shared_ptr<T> shared_from_this_T();

	private:
		std::string mStrKey;
		const UINT32 mID;

		static UINT32 gIDNext;
	};

	template<typename T>
	inline std::shared_ptr<T> Entity::shared_from_this_T()
	{
		static_assert(std::is_base_of_v<Entity, T>);
		return std::static_pointer_cast<T>(shared_from_this());
	}
}






