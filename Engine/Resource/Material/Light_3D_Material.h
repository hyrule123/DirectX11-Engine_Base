#pragma once
#include "Engine/Resource/Material/Material.h"

namespace ehw {
	class Light_3D_Material
		: public Material
	{
		CLASS_NAME(Light_3D_Material);
		REGISTER_INSTANCE_DEFAULT(Light_3D_Material);

	public:
		Light_3D_Material();
		virtual ~Light_3D_Material();


	};
}

