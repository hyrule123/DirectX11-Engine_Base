#pragma once
#include "Engine/define_Macro.h"
#include "Engine/define_Enum.h"

namespace ehw
{
	STR_KEY ComponentPrefix = "Com_";
	enum class eComponentCategory
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

		BehaviorTree,

		BaseComponentEnd,

		Scripts = BaseComponentEnd,

		END
	};
	inline bool IsComponentCategoryValid(eComponentCategory _cat)
	{
		return (_cat < eComponentCategory::END);
	}

	namespace name
	{
		STR_KEY eComponentCategory_String[(int)eComponentCategory::END] =
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

			"BehaviorTree",
			"Scripts"
		};
		
		inline STR_KEY GetComponentCategoryName(eComponentCategory _category)
		{
			return eComponentCategory_String[(int)_category];
		}
	}
}
