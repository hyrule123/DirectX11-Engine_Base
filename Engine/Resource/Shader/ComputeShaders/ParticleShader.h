#pragma once
#include "../iComputeShader.h"



#include "../../../GPU/StructBuffer.h"

namespace ehw
{
	class ParticleShader 
		: public iComputeShader
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
