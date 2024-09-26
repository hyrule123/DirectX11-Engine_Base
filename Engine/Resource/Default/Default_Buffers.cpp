#include "Engine/Resource/Default/DefaultResources.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace ehw {
	void DefaultResources::create_default_buffers() {
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
	}
}
