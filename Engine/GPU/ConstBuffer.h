#pragma once
#include "GPUBuffer.h"



#include "../DefaultShader/ConstBuffer.hlsli"

namespace ehw
{
	class ConstBuffer 
		: public GPUBuffer
	{
	public:
		ConstBuffer(eCBType _type);
		virtual ~ConstBuffer();

		template <typename T>
		bool Create(uint _dataCount = 1u);
		bool Create(size_t _dataSize, uint _dataCount = 1u);
		void SetData(void* _data, uint _dataCount = 1u);
		void BindData(eShaderStageFlag_ _stageFlag = eShaderStageFlag::NONE);

		void SetPresetTargetStage(eShaderStageFlag_ _flag) { mPresetTargetStage = _flag; }
	private:
		const eCBType m_ComCategory;

		uint				mDataSize;
		uint				mDataCount;
		eShaderStageFlag_	mPresetTargetStage;
	};


	template<typename T>
	inline bool ConstBuffer::Create(uint _dataCount)
	{
		return Create(sizeof(T), _dataCount);
	}
}
