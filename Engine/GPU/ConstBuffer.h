#pragma once
#include <Engine/GPU/GPUBuffer.h>

#include <Engine/DefaultShader/ConstBuffer.hlsli>

namespace core
{
	BASE_RESOURCE(ConstBuffer);
	class ConstBuffer 
		: public GPUBuffer
	{
		CLASS_INFO(ConstBuffer, GPUBuffer);
	public:
		ConstBuffer(uint _register);
		virtual ~ConstBuffer();

		template <typename T>
		bool create(uint _dataCount = 1u) {
			return create(sizeof(T), _dataCount);
		}
		bool create(size_t _dataSize, uint _dataCount = 1u);
		void set_data(const void* _data, uint _dataCount = 1u);
		void bind_buffer_to_GPU_register(eShaderStageFlag_ _override_flag = eShaderStageFlag::NONE);

		void unbind_buffer();

		void set_target_stage(eShaderStageFlag_ _flag) { m_target_stage = _flag; }
	private:
		const uint m_const_buffer_type;

		uint				m_data_size;
		uint				m_data_count;
		eShaderStageFlag_	m_target_stage;
	};
}
