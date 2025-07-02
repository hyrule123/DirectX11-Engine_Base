#pragma once
#include <Engine/define_Macro.h>
#include <Engine/define_Enum.h>

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

		Script,
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
}
