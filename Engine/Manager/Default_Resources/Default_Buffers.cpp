#include <Engine/Manager/ResourceManagers.h>
#include <Engine/Manager/ResourceManager.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/StructBuffer.h>
#include <Engine/Resource/Shader/ComputeShaders/GPUInitSetting.h>

#include <Engine/DefaultShader/Debug/Debug.hlsli>
#include <Engine/DefaultShader/Light/Light.hlsli>

namespace core {
	//함수명 = resourcemanager key
	void debug_render_buffer();
	void light_3D_instancing_buffer();
	void light_3D_const_buffer();
	void default_3D_material_instancing_buffer();

	void ResourceManagers::create_default_buffers()
	{
		s_ptr<GPUInitSetting> initSetting = ResourceManager<ComputeShader>::get_inst().load<GPUInitSetting>(name::defaultRes::shader::compute::GPUInitSetting);
		initSetting->on_execute();

		debug_render_buffer();
		light_3D_instancing_buffer();
		light_3D_const_buffer();
		default_3D_material_instancing_buffer();
	}
	void debug_render_buffer()
	{
		s_ptr<StructBuffer> debug_render_buffer = new_entity<StructBuffer>();
		debug_render_buffer->set_engine_default_res(true);

		StructBuffer::tDesc desc{};
		desc.m_buffer_RW_type = eStructBufferType::READ_ONLY;
		desc.m_SRV_target_register_idx = GPU::Register::t::g_debugDrawData;
		desc.m_SRV_target_stage = eShaderStageFlag::Vertex | eShaderStageFlag::Pixel;

		debug_render_buffer->create<tDebugDrawData>(desc, 100, nullptr, 0);

		ResourceManager<StructBuffer>::get_inst().insert("debug_render_buffer", debug_render_buffer);
	}
	void light_3D_instancing_buffer()
	{
		s_ptr<StructBuffer> light_3d_instancing_buffer = new_entity<StructBuffer>();
		light_3d_instancing_buffer->set_engine_default_res(true);
		StructBuffer::tDesc desc{};
		desc.m_buffer_RW_type = eStructBufferType::READ_ONLY;
		desc.m_SRV_target_register_idx = GPU::Register::t::g_light_attributes;
		desc.m_UAV_target_register_idx = GPU::Register::u::NONE;
		light_3d_instancing_buffer->create<tLightAttribute>(desc, 16);
		light_3d_instancing_buffer->set_pipeline_target(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);
		ResourceManager<StructBuffer>::get_inst().insert("light_3D_instancing_buffer", light_3d_instancing_buffer);
	}
	void light_3D_const_buffer()
	{
		s_ptr<ConstBuffer> light_3d_const_buffer = new_entity<ConstBuffer>(GPU::Register::b::g_CB_light_count);
		light_3d_const_buffer->set_engine_default_res(true);
		light_3d_const_buffer->create<tLightCount>();
		light_3d_const_buffer->set_target_stage(eShaderStageFlag::Vertex | eShaderStageFlag::Pixel);

		ResourceManager<ConstBuffer>::get_inst().insert("light_3D_const_buffer", light_3d_const_buffer);
	}
	void default_3D_material_instancing_buffer()
	{
		s_ptr<StructBuffer> buf = new_entity<StructBuffer>();
		buf->set_engine_default_res(true);
		StructBuffer::tDesc desc{};
		desc.m_buffer_RW_type = eStructBufferType::READ_ONLY;
		desc.m_SRV_target_register_idx = GPU::Register::t::g_default_3D_mtrl_instancing_buffer;
		desc.m_UAV_target_register_idx = GPU::Register::u::NONE;
		buf->create<tDefault3DMtrl_InstancingData>(desc);

		ResourceManager<StructBuffer>::get_inst().insert("default_3D_material_instancing_buffer", buf);
	}
}
