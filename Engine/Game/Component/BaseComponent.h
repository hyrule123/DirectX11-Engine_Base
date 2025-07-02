#pragma once
#include <Engine/Game/Component/Component.h>

namespace core
{
	template <typename T, eComponentOrder component_order>
	class BaseComponent
		: public Component
	{
	public:
		BaseComponent(const std::string_view key) : Component(key, type) {}
		virtual ~BaseComponent() {}

		template <typename U>
		constexpr bool is_base_component() {
			return std::is_base_of_v<T, U>;
		}
		constexpr eComponentOrder get_component_order() { return component_order; }

	private:
		constexpr inline static const eComponentOrder s_component_order = component_order;
	};
}
