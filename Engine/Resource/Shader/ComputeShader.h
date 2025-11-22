#pragma once
#include "Engine/Resource/Shader/Shader.h"
#include "Engine/GPU/CommonGPU.h"

//1. bind_buffer_to_GPU_register() 함수를 재정의해서 필요한 데이터를 바인딩하도록 설정
//2. Clear() 함수를 재정의해서 데이터 바인딩을 해제하는 함수도 재정의
//3. 데이터의 갯수 계산. 이건 처음에 한번만 계산해주면 될 때도 있고 매번 새로 계산해줘야할때도 있음.
//		매번 새로 계산해줘야 할 경우 bind_buffer_to_GPU_register()에서 호출해주면 되고, 한번만 해주면 될 경우에는 생성자에서 해주면 됨
//4. 위의 3개 완료했으면 필요한 시점에 on_execute() 호출

namespace core
{
	class ComputeShader 
		: public Shader
	{
		CLASS_INFO(ComputeShader, Shader);

	public:
		ComputeShader(uint3 _threadsPerGroup);
		virtual ~ComputeShader();

		void calculate_group_count(const uint3& _dataCounts);

		bool on_execute();

	protected:
		virtual bool load_shader(const std::vector<unsigned char>& _byte_code) final;

		virtual bool bind_buffer_to_GPU_register() = 0;
		virtual void unbind_buffer_from_GPU_register() = 0;

	private:
		ComPtr<ID3D11ComputeShader> m_particle_CS = {};

		//스레드 갯수, 그룹 갯수 등 여기 저장
		tCB_ComputeShader m_compute_shader_cbuffer = {};
	};
}
