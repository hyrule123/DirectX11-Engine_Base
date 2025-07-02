#pragma once
#include <Engine/Resource/Material/Material.h>

namespace core {
	class Light_3D_Material
		: public Material
	{
		CLASS_INFO(Light_3D_Material, Material);
		REGISTER_FACTORY(Light_3D_Material);

	public:
		Light_3D_Material();
		virtual ~Light_3D_Material();


	};
}

