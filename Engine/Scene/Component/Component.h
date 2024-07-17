#pragma once
#include "Engine/Scene/Component/iComponent.h"
#include <type_traits>

namespace ehw
{

	class iComponent;

	//제한적인 업캐스팅 지원을 위한 클래스 - 상속받는 클래스들은 이 클래스를 상속
	//(iComponent를 직접 상속하지 말것)
	template <class BaseComponentType, eComponentCategory _category>
	class Component : public iComponent
	{
	public:
		Component() : iComponent(_category) {};
		virtual ~Component() {};

		template <class CompareType>
		static constexpr bool IsBaseComponentType()
		{
			return std::is_same_v<iComponent, CompareType> || std::is_same_v<BaseComponentType, CompareType>;
		}

		static constexpr eComponentCategory GetComponentCategoryStatic() { return category; }
	private:
		static constexpr eComponentCategory category = _category;
	};
}
