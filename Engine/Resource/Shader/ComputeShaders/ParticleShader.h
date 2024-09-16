#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"

namespace ehw
{
	class StructBuffer;
	class ParticleShader 
		: public ComputeShader
	{
		CLASS_NAME(ParticleShader);
	public:
		ParticleShader();
		virtual ~ParticleShader();

		virtual bool bind_buffer_to_GPU_register() override;
		virtual void unbind_buffer_from_GPU_register() override;

		void SetStrcutedBuffer(StructBuffer* _buffer) { mBuffer = _buffer; }
		void SetSharedStrutedBuffer(StructBuffer* _buffer) { mSharedBuffer = _buffer; }

	private:
		StructBuffer* mBuffer;
		StructBuffer* mSharedBuffer;
	};
}
