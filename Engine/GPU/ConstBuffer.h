#pragma once
#include "Engine/GPU/GPUBuffer.h"

#include "Engine/DefaultShader/ConstBuffer.hlsli"

namespace ehw
{
	class ConstBuffer 
		: public GPUBuffer
	{
		CLASS_NAME(ConstBuffer);
	public:
		ConstBuffer(uint _register);
		virtual ~ConstBuffer();

		template <typename T>
		bool create(uint _dataCount = 1u);
		bool create(size_t _dataSize, uint _dataCount = 1u);
		void SetData(void* _data, uint _dataCount = 1u);
		void bind_buffer_to_GPU_register(eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

		void unbind_data();

		void SetPresetTargetStage(eShaderStageFlag_ _flag) { mPresetTargetStage = _flag; }
	private:
		const uint m_constBufferType;

		uint				mDataSize;
		uint				mDataCount;
		eShaderStageFlag_	mPresetTargetStage;
	};


	template<typename T>
	inline bool ConstBuffer::create(uint _dataCount)
	{
		return create(sizeof(T), _dataCount);
	}
}
