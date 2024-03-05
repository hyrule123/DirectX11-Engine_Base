#include "ResourceManagers.h"

#include "Util/AtExit.h"

#include "PathManager.h"
#include "ResourceManager.h"

#include "Resource/Texture.h"
#include "Resource/Mesh.h"
#include "Resource/Material.h"
#include "Resource/Model3D/Model3D.h"
#include "Resource/Model3D/Skeleton.h"
#include "Resource/AudioClip.h"
#include "Resource/Animation2D.h"
#include "Resource/Shader/GraphicsShader.h"
#include "Resource/Shader/iComputeShader.h"


namespace ehw
{
	std::vector<std::function<void()>> ResourceManagers::m_CleanUnusedResourcesFunction{};

	void ResourceManagers::CleanUnusedResources()
	{
		//거꾸로 실행
		for (size_t i = m_CleanUnusedResourcesFunction.size() - (size_t)1; i >= 0 ; --i)
		{
			m_CleanUnusedResourcesFunction[i]();
		}
	}

	void ResourceManagers::Init()
	{
		AtExit::AddFunc(Release);

		//기본 Resource들 init
		const std::fs::path& baseDir = PathManager::GetResPathRelative();

		//기본 리소스를 먼저 등록
		ResourceManager<Texture>::Init(baseDir / strKey::path::directory::resource::Texture);
		ResourceManager<Mesh>::Init(baseDir / strKey::path::directory::resource::Mesh);
		ResourceManager<AudioClip>::Init(baseDir / strKey::path::directory::resource::AudioClip);
		ResourceManager<GraphicsShader>::Init(baseDir / strKey::path::directory::resource::GraphicsShader);
		ResourceManager<iComputeShader>::Init(baseDir / strKey::path::directory::resource::ComputeShader);


		//다른 리소스를 참조하는 리소스를 나중에 등록
		ResourceManager<Material>::Init(baseDir / strKey::path::directory::resource::Material);
		ResourceManager<Animation2D>::Init(baseDir / strKey::path::directory::resource::Animation2D);
		ResourceManager<Model3D>::Init(baseDir / strKey::path::directory::resource::Model3D);
		ResourceManager<Skeleton>::Init(baseDir / strKey::path::directory::resource::Model3D);
	}

	void ResourceManagers::Release()
	{
		m_CleanUnusedResourcesFunction.clear();
	}

}


