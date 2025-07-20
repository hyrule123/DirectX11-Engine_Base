#pragma once
#include <Engine/Resource/Shader/Shader.h>

#include <Engine/GPU/CommonGPU.h>

//1. bind_buffer_to_GPU_register() 함수를 재정의해서 필요한 데이터를 바인딩하도록 설정
//2. Clear() 함수를 재정의해서 데이터 바인딩을 해제하는 함수도 재정의
//3. 데이터의 갯수 계산. 이건 처음에 한번만 계산해주면 될 때도 있고 매번 새로 계산해줘야할때도 있음.
//		매번 새로 계산해줘야 할 경우 bind_buffer_to_GPU_register()에서 호출해주면 되고, 한번만 해주면 될 경우에는 생성자에서 해주면 됨
//4. 위의 3개 완료했으면 필요한 시점에 on_execute() 호출

namespace core
{
	BASE_RESOURCE(ComputeShader);
	class ComputeShader 
		: public Shader
	{
		CLASS_INFO(ComputeShader, Shader);
	public:
		ComputeShader(const std::string_view key, uint3 _threadsPerGroup);
		virtual ~ComputeShader();

		//컴퓨트쉐이더의 경우 리소스이지만 다형성(상속관계)가 필요하기 때문에
		//다른 리소스와는 로드 방식이 다름.
		//이 클래스를 상속한 뒤,
		//Load 함수와 bind_buffer_to_GPU_register, unbind_buffer 함수를 재정의 해준뒤
		//ResourceMgr를 통해서 '해당 클래스를' 로드해준다.
		//이때 키값은 왠만하면 클래스명으로 지어주는것을 추천
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		eResult compile_from_source_code(const std::filesystem::path& _FullPath, const std::string_view _funcName);

		eResult compile_from_byte_code(const unsigned char* _pByteCode, size_t _ByteCodeSize);
		eResult compile_from_CSO(const std::filesystem::path& _FileName);

		void calculate_group_count(const uint3& _dataCounts);

		bool on_execute();

	protected:
		virtual bool bind_buffer_to_GPU_register() = 0;
		virtual void unbind_buffer_from_GPU_register() = 0;

		eResult create_shader(const void* _pByteCode, size_t _ByteCodeSize);

	private:
		Microsoft::WRL::ComPtr<ID3DBlob> m_CS_blob;
		Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_particle_CS;

		//스레드 갯수, 그룹 갯수 등 여기 저장
		tCB_ComputeShader m_compute_shader_cbuffer;
	};
}
