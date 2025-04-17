#include "Engine/Manager/ResourceManagers.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"
#include "Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h"

#include "Engine/DefaultShader/Debug/Debug.hlsli"
#include "Engine/DefaultShader/Light/Light.hlsli"

namespace core {
	//함수명 = resourcemanager key
	void debug_render_buffer();
	void light_3D_instancing_buffer();
	void light_3D_const_buffer();
	void default_3D_material_instancing_buffer();

	void ResourceManagers::create_default_buffers()
	{
		std::shared_ptr<GPUInitSetting> initSetting = ResourceManager<ComputeShader>::get_inst().load<GPUInitSetting>(name::defaultRes::shader::compute::GPUInitSetting);
		initSetting->on_execute();

		debug_render_buffer();
		light_3D_instancing_buffer();
		light_3D_const_buffer();
		default_3D_material_instancing_buffer();
	}
	void debug_render_buffer()
	{
		std::shared_ptr<StructBuffer> debug_render_buffer = std::make_shared<StructBuffer>();
		debug_render_buffer->set_engine_default_res(true);

		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_debugDrawData;
		desc.TargetStageSRV = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		debug_render_buffer->init<tDebugDrawData>(desc, 100, nullptr, 0);

		ResourceManager<StructBuffer>::get_inst().insert("debug_render_buffer", debug_render_buffer);
	}
	void light_3D_instancing_buffer()
	{
		std::shared_ptr<StructBuffer> light_3d_instancing_buffer = std::make_shared<StructBuffer>();
		light_3d_instancing_buffer->set_engine_default_res(true);
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_light_attributes;
		desc.GPU_register_u_UAV = GPU::Register::u::NONE;
		light_3d_instancing_buffer->init<tLightAttribute>(desc, 16);
		light_3d_instancing_buffer->SetPipelineTarget(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
		ResourceManager<StructBuffer>::get_inst().insert("light_3D_instancing_buffer", light_3d_instancing_buffer);
	}
	void light_3D_const_buffer()
	{
		std::shared_ptr<ConstBuffer> light_3d_const_buffer = std::make_shared<ConstBuffer>(GPU::Register::b::g_CB_light_count);
		light_3d_const_buffer->set_engine_default_res(true);
		light_3d_const_buffer->create<tLightCount>();
		light_3d_const_buffer->SetPresetTargetStage(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		ResourceManager<ConstBuffer>::get_inst().insert("light_3D_const_buffer", light_3d_const_buffer);
	}
	void default_3D_material_instancing_buffer()
	{
		std::shared_ptr<StructBuffer> buf = std::make_shared<StructBuffer>();
		buf->set_engine_default_res(true);
		StructBuffer::Desc desc{};
		desc.eSBufferType = eStructBufferType::READ_ONLY;
		desc.GPU_register_t_SRV = GPU::Register::t::g_default_3D_mtrl_instancing_buffer;
		desc.GPU_register_u_UAV = GPU::Register::u::NONE;
		buf->init<tDefault3DMtrl_InstancingData>(desc);

		ResourceManager<StructBuffer>::get_inst().insert("default_3D_material_instancing_buffer", buf);
	}
}
