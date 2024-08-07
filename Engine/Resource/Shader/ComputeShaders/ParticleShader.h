#pragma once
#include "Engine/Resource/Shader/ComputeShader.h"

namespace ehw
{
	class StructBuffer;
	class ParticleShader 
		: public ComputeShader
	{
	public:
		ParticleShader();
		virtual ~ParticleShader();

		virtual bool BindData() override;
		virtual void UnBindData() override;

		void SetStrcutedBuffer(StructBuffer* _buffer) { mBuffer = _buffer; }
		void SetSharedStrutedBuffer(StructBuffer* _buffer) { mSharedBuffer = _buffer; }

	private:
		StructBuffer* mBuffer;
		StructBuffer* mSharedBuffer;
	};
}
