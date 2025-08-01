#pragma once
#include <Engine/Resource/Shader/ComputeShader.h>

namespace core
{
	class StructBuffer;
	class ParticleShader 
		: public ComputeShader
	{
		CLASS_INFO(ParticleShader, ComputeShader);
	public:
		ParticleShader();
		virtual ~ParticleShader();

		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

		void set_struct_buffer(const s_ptr<StructBuffer>& _buffer) { m_buffer = _buffer; }
		void set_shared_struct_buffer(const s_ptr<StructBuffer>& _buffer) { m_shared_buffer = _buffer; }

	private:
		s_ptr<StructBuffer> m_buffer;
		s_ptr<StructBuffer> m_shared_buffer;
	};
}
