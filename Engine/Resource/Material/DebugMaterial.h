#pragma once
#include <Engine/Resource/Material/Material.h>

#include <Engine/DefaultShader/Debug/Debug.hlsli>

namespace core {
	class DebugMaterial
		: public Material
	{
		CLASS_INFO(DebugMaterial, Material);
		REGISTER_FACTORY(DebugMaterial);
	public:
		DebugMaterial();
		virtual ~DebugMaterial();

		void set_data_and_bind_GPU(const std::vector<tDebugDrawData>& _data);

		//미사용//
		void clear_instancing_buffer() final {}; //버퍼 클리어
		void set_data_to_instancing_buffer(const std::vector<s_ptr<GameObject>>& _objs) final {};	//미사용
		void bind_instancing_buffer_to_GPU_register() final {};
		///////////////

	private:

		s_ptr<StructBuffer> m_debug_render_buffer;
	};
}


