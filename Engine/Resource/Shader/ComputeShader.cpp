
#include "ComputeShader.h"


#include <Engine/Manager/ResourceManager.h>

#include <Engine/Manager/RenderManager.h>

#include <Engine/GPU/ConstBuffer.h>
#include <Engine/GPU/CommonGPU.h>

#include <Engine/Resource/define_Resource.h>

#include <fstream>

namespace core
{
	ComputeShader::ComputeShader(uint3 _threadsPerGroup)
		: m_compute_shader_cbuffer{ _threadsPerGroup,  }
	{
		//스레드가 하나라도 0이 들어있으면 안됨
		ASSERT_MESSAGE(_threadsPerGroup.x && _threadsPerGroup.y && _threadsPerGroup.z, "컴퓨트쉐이더 Thread Axis 중 하나에 0이 들어가 있습니다.");
	}

	ComputeShader::~ComputeShader()
	{
	}

	void ComputeShader::calculate_group_count(const uint3& _dataCounts)
	{
		m_compute_shader_cbuffer.TotalDataCount = _dataCounts;

		//필요한 그룹 수를 계산.
		m_compute_shader_cbuffer.NumGroup.x = m_compute_shader_cbuffer.TotalDataCount.x / m_compute_shader_cbuffer.ThreadsPerGroup.x;
		if (0u == m_compute_shader_cbuffer.NumGroup.x)
		{
			m_compute_shader_cbuffer.NumGroup.x = 1u;
		}

		m_compute_shader_cbuffer.NumGroup.y = m_compute_shader_cbuffer.TotalDataCount.y / m_compute_shader_cbuffer.ThreadsPerGroup.y;
		if (0u == m_compute_shader_cbuffer.NumGroup.y)
		{
			m_compute_shader_cbuffer.NumGroup.y = 1u;
		}

		m_compute_shader_cbuffer.NumGroup.z = m_compute_shader_cbuffer.TotalDataCount.z / m_compute_shader_cbuffer.ThreadsPerGroup.z;
		if (0u == m_compute_shader_cbuffer.NumGroup.z)
		{
			m_compute_shader_cbuffer.NumGroup.z = 1u;
		}
	}

	bool ComputeShader::on_execute()
	{
		if (false == bind_buffer_to_GPU_register()) {
			return false; 
		}

		//데이터 카운트가 하나라도 0일경우 계산 불가
		 if (false == (
			 m_compute_shader_cbuffer.TotalDataCount.x && 
			 m_compute_shader_cbuffer.TotalDataCount.y && 
			 m_compute_shader_cbuffer.TotalDataCount.z))
		 {
			 unbind_buffer_from_GPU_register();
			 return false;
		 }
		
		//상수버퍼를 통해 데이터 수를 업로드
		static ConstBuffer* const pCB = RenderManager::get_inst().get_const_buffer(eCBType::ComputeShader);
		pCB->set_data(&m_compute_shader_cbuffer);
		pCB->bind_buffer_to_GPU_register();

		auto context = RenderManager::get_inst().get_context();
		//쉐이더 바인딩
		context->CSSetShader(m_particle_CS.Get(), nullptr, 0);
		context->Dispatch(m_compute_shader_cbuffer.NumGroup.x, m_compute_shader_cbuffer.NumGroup.y, m_compute_shader_cbuffer.NumGroup.z);

		//데이터 정리
		unbind_buffer_from_GPU_register();

		return true;
	}

	bool ComputeShader::load_shader(const std::vector<unsigned char>& _byte_code)
	{
		auto device = RenderManager::get_inst().get_device();

		HRESULT result = device->CreateComputeShader(
				_byte_code.data(),
				_byte_code.size(),
				nullptr,
				m_particle_CS.ReleaseAndGetAddressOf()
		);

		if (FAILED(result))
		{
			ERROR_MESSAGE("컴퓨트 쉐이더 생성에 실패했습니다.");
			return false;
		}

		return true;
	}

}
