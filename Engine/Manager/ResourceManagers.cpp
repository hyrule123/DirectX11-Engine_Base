#include "ResourceManagers.h"

#include "Engine/Util/AtExit.h"

#include "PathManager.h"
#include "ResourceManager.h"

#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"
#include "Engine/Resource/Model3D/Model3D.h"
#include "Engine/Resource/Model3D/Skeleton.h"
#include "Engine/Resource/AudioClip.h"
#include "Engine/Resource/Animation2D.h"
#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/Resource/Shader/ComputeShader.h"
#include "Engine/Resource/Mesh/VertexBuffer.h"
#include "Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Light/Light_3D.h"


namespace core
{
	ResourceManagers::ResourceManagers()
		: m_CleanUnusedResourcesFunction{}
	{
		AtExit::add_func(ResourceManagers::destroy_inst);
	}

	ResourceManagers::~ResourceManagers()
	{
		Transform::release_static();
		m_CleanUnusedResourcesFunction.clear();
	}

	void ResourceManagers::CleanUnusedResources()
	{
		//거꾸로 실행
		for (size_t i = m_CleanUnusedResourcesFunction.size() - (size_t)1; i >= 0 ; --i)
		{
			m_CleanUnusedResourcesFunction[i]();
		}
	}

	void ResourceManagers::init_resource_managers()
	{
		//기본 Resource들 init
		const std::fs::path& baseDir = PathManager::get_inst().GetResPathRelative();

		ResourceManager<VertexBuffer>::get_inst().init(baseDir / name::path::directory::resource::Mesh);
		ResourceManager<Texture>::get_inst().init(baseDir / name::path::directory::resource::Texture);
		ResourceManager<AudioClip>::get_inst().init(baseDir / name::path::directory::resource::AudioClip);
		ResourceManager<GraphicsShader>::get_inst().init(baseDir / name::path::directory::resource::GraphicsShader);
		ResourceManager<ComputeShader>::get_inst().init(baseDir / name::path::directory::resource::ComputeShader);

		//기본 리소스를 먼저 등록
		ResourceManager<ConstBuffer>::get_inst().init("");
		ResourceManager<StructBuffer>::get_inst().init("");

		//다른 리소스를 참조하는 리소스를 나중에 등록
		ResourceManager<Mesh>::get_inst().init(baseDir / name::path::directory::resource::Mesh);
		ResourceManager<Material>::get_inst().init(baseDir / name::path::directory::resource::Material);
		ResourceManager<Animation2D>::get_inst().init(baseDir / name::path::directory::resource::Animation2D);
		ResourceManager<Model3D>::get_inst().init(baseDir / name::path::directory::resource::Model3D);
		ResourceManager<Skeleton>::get_inst().init(baseDir / name::path::directory::resource::Model3D);
	}

	void ResourceManagers::load_default_resources()
	{
		create_default_buffers();
		load_default_shaders();
		load_default_textures();
		load_default_meshes();
		load_default_materials();

		Transform::init_static();
	}

}


