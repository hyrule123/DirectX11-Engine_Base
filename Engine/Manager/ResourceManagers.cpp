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

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"

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

	void ResourceManagers::init()
	{
		AtExit::AddFunc(std::bind(&ResourceManagers::release, this));

		//기본 Resource들 init
		const std::fs::path& baseDir = PathManager::get_inst().GetResPathRelative();

		//기본 리소스를 먼저 등록
		ResourceManager<ConstBuffer>::get_inst().init("");
		ResourceManager<StructBuffer>::get_inst().init("");

		{
			std::shared_ptr<StructBuffer> debug_render_buffer = std::make_shared<StructBuffer>();

			StructBuffer::Desc desc{};
			desc.eSBufferType = eStructBufferType::READ_ONLY;
			desc.GPU_register_t_SRV = GPU::Register::t::g_debugDrawData;
			desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

			debug_render_buffer->init<tDebugDrawData>(desc, 100, nullptr, 0);

			ResourceManager<StructBuffer>::get_inst().insert("debug_render_buffer", debug_render_buffer);
		}

		//이 부분 나중에 분리한 별도 CPP로 옮길 것
		std::shared_ptr<StructBuffer> light_3d_instancing_buffer = std::make_shared<StructBuffer>();
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_light_attributes;
		desc.GPU_register_u_UAV = GPU::Register::u::NONE;
		light_3d_instancing_buffer->init<tLightAttribute>(desc, 16);
		light_3d_instancing_buffer->SetPipelineTarget(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
		ResourceManager<StructBuffer>::get_inst().insert("light_3D_instancing_buffer", light_3d_instancing_buffer);

		std::shared_ptr<ConstBuffer> light_3d_const_buffer = std::make_shared<ConstBuffer>(GPU::Register::b::g_CB_light_count);
		light_3d_const_buffer->create<tLightCount>();
		light_3d_const_buffer->SetPresetTargetStage(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		ResourceManager<ConstBuffer>::get_inst().insert("light_3D_const_buffer", light_3d_const_buffer);
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		ResourceManager<Texture>::get_inst().init(baseDir / name::path::directory::resource::Texture);
		ResourceManager<VertexBuffer>::get_inst().init(baseDir / name::path::directory::resource::Mesh);
		ResourceManager<Mesh>::get_inst().init(baseDir / name::path::directory::resource::Mesh);
		ResourceManager<AudioClip>::get_inst().init(baseDir / name::path::directory::resource::AudioClip);
		ResourceManager<GraphicsShader>::get_inst().init(baseDir / name::path::directory::resource::GraphicsShader);
		ResourceManager<ComputeShader>::get_inst().init(baseDir / name::path::directory::resource::ComputeShader);


		//다른 리소스를 참조하는 리소스를 나중에 등록
		ResourceManager<Material>::get_inst().init(baseDir / name::path::directory::resource::Material);
		ResourceManager<Animation2D>::get_inst().init(baseDir / name::path::directory::resource::Animation2D);
		ResourceManager<Model3D>::get_inst().init(baseDir / name::path::directory::resource::Model3D);
		ResourceManager<Skeleton>::get_inst().init(baseDir / name::path::directory::resource::Model3D);

		init_static_variables();
	}

	void ResourceManagers::release()
	{
		release_static_variables();
		m_CleanUnusedResourcesFunction.clear();
	}

	void ResourceManagers::init_static_variables()
	{
		Transform::init_static();
	}

	void ResourceManagers::release_static_variables()
	{
		Transform::release_static();
	}

}


