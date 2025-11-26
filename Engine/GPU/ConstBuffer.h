#pragma once
#include <Engine/GPU/GPUBuffer.h>

#include <Engine/Shader/ConstBuffer.hlsli>

namespace core
{
	class ConstBuffer 
		: public GPUBuffer
	{
		CLASS_INFO(ConstBuffer, GPUBuffer);
		BASE_RESOURCE(ConstBuffer, eResourceType::ConstBuffer);

	public:
		ConstBuffer();
		virtual ~ConstBuffer();

		template <typename T>
		bool create(UINT _target_register, UINT _dataCount = 1u) {
			return create(sizeof(T), _dataCount);
		}
		bool create(UINT _target_register, UINT _dataSize, UINT _dataCount = 1u);
		void set_data(const void* _data, uint _dataCount = 1u);
		void bind(eShaderStageFlag_ _stage_flag = eShaderStageFlag::ALL);

		void unbind();
	private:
		virtual eResult save(const std::fs::path& _fullpath) const override;
		virtual eResult load(const std::fs::path& _fullpath) override;

		UINT m_target_slot = {};

		UINT				m_data_size = {};
		UINT				m_data_count = {};

		static inline ComPtr<ID3D11Buffer> s_bound_table[(int)eShaderStage::END][D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = {};
	};
}
