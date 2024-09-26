#include "DefaultResources.h"

#include "Engine/Game/Component/Transform.h"

namespace ehw {
	DefaultResources::DefaultResources()
	{
		create_default_buffers();
		load_default_mesh();
		load_default_shaders();
		load_default_textures();
		load_default_materials();

		Transform::init_static();
	}

	DefaultResources::~DefaultResources()
	{


		Transform::release_static();
	}
}

