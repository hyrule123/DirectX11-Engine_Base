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

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Light/Light_3D.h"

namespace ehw
{
	void ResourceManagers::CleanUnusedResources()
	{
		//거꾸로 실행
		for (size_t i = m_CleanUnusedResourcesFunction.size() - (size_t)1; i >= 0 ; --i)
		{
			m_CleanUnusedResourcesFunction[i]();
		}
	}

	ResourceManagers::ResourceManagers()
		: m_CleanUnusedResourcesFunction{}
	{
	}

	ResourceManagers::~ResourceManagers()
	{
	}

	void ResourceManagers::Init()
	{
		AtExit::AddFunc(std::bind(&ResourceManagers::Release, this));

		//기본 Resource들 init
		const std::fs::path& baseDir = PathManager::GetInst().GetResPathRelative();

		//기본 리소스를 먼저 등록
		ResourceManager<Texture>::GetInst().Init(baseDir / strKey::path::directory::resource::Texture);
		ResourceManager<VertexBuffer>::GetInst().Init(baseDir / strKey::path::directory::resource::Mesh);
		ResourceManager<Mesh>::GetInst().Init(baseDir / strKey::path::directory::resource::Mesh);
		ResourceManager<AudioClip>::GetInst().Init(baseDir / strKey::path::directory::resource::AudioClip);
		ResourceManager<GraphicsShader>::GetInst().Init(baseDir / strKey::path::directory::resource::GraphicsShader);
		ResourceManager<ComputeShader>::GetInst().Init(baseDir / strKey::path::directory::resource::ComputeShader);


		//다른 리소스를 참조하는 리소스를 나중에 등록
		ResourceManager<Material>::GetInst().Init(baseDir / strKey::path::directory::resource::Material);
		ResourceManager<Animation2D>::GetInst().Init(baseDir / strKey::path::directory::resource::Animation2D);
		ResourceManager<Model3D>::GetInst().Init(baseDir / strKey::path::directory::resource::Model3D);
		ResourceManager<Skeleton>::GetInst().Init(baseDir / strKey::path::directory::resource::Model3D);

		init_static_variables();
	}

	void ResourceManagers::Release()
	{
		release_static_variables();
		m_CleanUnusedResourcesFunction.clear();
	}

	void ResourceManagers::init_static_variables()
	{
		Transform::init_static();
		Light_3D::init_static();
		Material::init_static();
	}

	void ResourceManagers::release_static_variables()
	{
		Material::release_static();
		Light_3D::release_static();
		Transform::release_static();
	}

}


