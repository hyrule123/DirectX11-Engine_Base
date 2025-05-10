#pragma once
#include "Engine/Game/Component/Component.h"
#include <type_traits>

namespace core
{
	//업캐스팅 지원 및 enum과 연동하기 위한 클래스
	template <class BaseComponentType, eComponentCategory baseComponentOrder>
	class BaseComponent : public Component
	{
	public:
		BaseComponent(const std::string_view key) : Component(key, baseComponentOrder) {};
		virtual ~BaseComponent() {};

		template <class CompareType>
		static constexpr bool IsBaseComponentType()
		{
			return std::is_base_of_v<Component, CompareType> && std::is_same_v<BaseComponentType, CompareType>;
		}

		static constexpr const eComponentCategory s_order = baseComponentOrder;
	};
}
