#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

//컴포넌트 순서와 Base Component 등록

namespace core
{
	STR_KEY ComponentPrefix = "Com_";
	enum class eComponentOrder
	{
		Transform,
		Rigidbody,
		Collider,
		Animator,
		Light,
		Camera,
		Renderer,

		AudioSource,
		AudioListener,

		StateMachine,

		BaseComponentEnd,

		Script = BaseComponentEnd,

		END
	};
	inline bool IsComponentCategoryValid(eComponentOrder _cat)
	{
		return (_cat < eComponentOrder::END);
	}	

	namespace name
	{
		STR_KEY eComponentCategory_String[(int)eComponentOrder::END] =
		{
			"Transform",
			"Rigidbody",
			"Collider",
			"Animator",
			"Light",
			"Camera",
			"Renderer",

			"AudioSource",
			"AudioListener",

			"StateMachine",
			"Script"
		};
		
		inline STR_KEY GetComponentCategoryName(eComponentOrder _category)
		{
			return eComponentCategory_String[(int)_category];
		}
	}

	template <typename T>
	struct BaseComponent {
		constexpr static inline const eComponentOrder s_order = eComponentOrder::END;
	};

	template <typename T>
	constexpr bool is_base_component()
	{
		return BaseComponent<T>::s_order != eComponentOrder::END;
	}

#define BASE_COMPONENT(_class_, _component_order_) \
template <>\
struct BaseComponent<class _class_> {\
	constexpr static inline const eComponentOrder s_order = _component_order_;\
}
	BASE_COMPONENT(Transform, eComponentOrder::Transform);
	BASE_COMPONENT(Rigidbody, eComponentOrder::Rigidbody);
	BASE_COMPONENT(Collider, eComponentOrder::Collider);
	BASE_COMPONENT(Animator, eComponentOrder::Animator);
	BASE_COMPONENT(Light, eComponentOrder::Light);
	BASE_COMPONENT(Com_Camera, eComponentOrder::Camera);
	BASE_COMPONENT(Renderer, eComponentOrder::Renderer);
	BASE_COMPONENT(Com_AudioSource, eComponentOrder::AudioSource);
	BASE_COMPONENT(Com_AudioListener, eComponentOrder::AudioListener);
	BASE_COMPONENT(StateMachine, eComponentOrder::StateMachine);
}
