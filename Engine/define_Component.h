#pragma once
#include "define_Macro.h"
#include "define_Enum.h"
#include "strKey_Component.h"

namespace ehw
{
	STRKEY ComponentPrefix = "Com_";
	enum class eComponentType
	{
		UNKNOWN = -1,

		Transform,
		Collider,
		Animator,
		Light,
		Camera,
		Renderer,

		AudioSource,
		AudioListener,

		BehaviorTree,
		Scripts,

		END
	};

	namespace strKey
	{
		STRKEY eComponentType_String[(int)eComponentType::END] =
		{
			"Transform",
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
	}
}
