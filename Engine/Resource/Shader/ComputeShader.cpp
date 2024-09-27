#include "Engine/Resource/Shader/ComputeShader.h"


#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/PathManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/CommonGPU.h"


#include "Engine/Resource/define_Resource.h"

#include <fstream>

namespace ehw
{
	ComputeShader::ComputeShader(const std::string_view key, uint3 _threadsPerGroup)
		: Shader(key)
		, m_CSBlob(nullptr)
		, m_CS(nullptr)
		, mCB_ComputeShader{ _threadsPerGroup,  }
	{
		//스레드가 하나라도 0이 들어있으면 안됨
		ASSERT(_threadsPerGroup.x && _threadsPerGroup.y && _threadsPerGroup.z, "컴퓨트쉐이더 Thread Axis 중 하나에 0이 들어가 있습니다.");
	}

	ComputeShader::~ComputeShader()
	{
	}

	eResult ComputeShader::load(const std::fs::path& _base_directory, const std::fs::path& _resource_name)
	{
		return compile_from_CSO(_base_directory / _resource_name);
	}

	eResult ComputeShader::compile_from_source_code(const std::filesystem::path& _FullPath, const std::string_view _funcName)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob = nullptr;

		if (FAILED(D3DCompileFromFile(
			_FullPath.wstring().c_str(), 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			std::string(_funcName).c_str(), 
			ehw::SHADER_VERSION::Compute, 
			0, 
			0, 
			m_CSBlob.GetAddressOf(), 
			mErrorBlob.GetAddressOf())))
		{
			std::string ErrMsg = "Failed to compile Compute GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(mErrorBlob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());

			mErrorBlob = nullptr;

			return eResult::Fail_Open;
		}


		return eResult::Success;
	}

	eResult ComputeShader::compile_from_byte_code(const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		set_engine_default_res(true);

		HRESULT hr = D3DCreateBlob(_ByteCodeSize, m_CSBlob.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ERROR_MESSAGE("GraphicsShader를 저장할 Blob 생성에 실패했습니다.");

			return eResult::Fail_Create;
		}

		unsigned char* pCode = reinterpret_cast<unsigned char*>(m_CSBlob->GetBufferPointer());
		//할당된 메모리 사이즈는 무조건 같아야 함
		size_t DestSize = m_CSBlob->GetBufferSize();
		ASSERT(_ByteCodeSize == DestSize, "할당된 메모리와 쉐이더 byte code의 사이즈가 다릅니다.");

		//데이터 복사
		memcpy_s(pCode, DestSize, _pByteCode, _ByteCodeSize);

		return create_shader(pCode, DestSize);
	}

	eResult ComputeShader::compile_from_CSO(const std::filesystem::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathManager::get_inst().GetShaderCSOPath();
		shaderBinPath /= _FileName;

		//위에서 만든 파일명을 토대로 디스크에서 파일을 열어준다.(뒤에서부터)
		std::ios_base::openmode openFlag = std::ios_base::ate | std::ios_base::binary; std::ios_base::in;
		std::ifstream sFile(shaderBinPath, openFlag);

		if (false == sFile.is_open())
		{
			std::wstring msg = L"Failed to open File.\n";
			msg += shaderBinPath.wstring();
			ERROR_MESSAGE_W(msg.c_str());
		}

		//Blob 내부에 공간을 할당.
		bool result = SUCCEEDED(D3DCreateBlob(sFile.tellg(), m_CSBlob.GetAddressOf()));
		ASSERT(result, "쉐이더를 저장할 공간 할당에 실패했습니다.");
		

		//커서를 처음으로 돌린 후 파일을 읽어준다.
		sFile.seekg(0, std::ios_base::beg);
		sFile.read((char*)m_CSBlob->GetBufferPointer(), m_CSBlob->GetBufferSize());
		sFile.close();

		unsigned char* pCode = reinterpret_cast<unsigned char*>(m_CSBlob->GetBufferPointer());

		return create_shader(pCode, m_CSBlob->GetBufferSize());
	}

	void ComputeShader::calculate_group_count(const uint3& _dataCounts)
	{
		mCB_ComputeShader.TotalDataCount = _dataCounts;

		//필요한 그룹 수를 계산.
		mCB_ComputeShader.NumGroup.x = mCB_ComputeShader.TotalDataCount.x / mCB_ComputeShader.ThreadsPerGroup.x;
		if (0u == mCB_ComputeShader.NumGroup.x)
			mCB_ComputeShader.NumGroup.x = 1u;

		mCB_ComputeShader.NumGroup.y = mCB_ComputeShader.TotalDataCount.y / mCB_ComputeShader.ThreadsPerGroup.y;
		if (0u == mCB_ComputeShader.NumGroup.y)
			mCB_ComputeShader.NumGroup.y = 1u;

		mCB_ComputeShader.NumGroup.z = mCB_ComputeShader.TotalDataCount.z / mCB_ComputeShader.ThreadsPerGroup.z;
		if (0u == mCB_ComputeShader.NumGroup.z)
			mCB_ComputeShader.NumGroup.z = 1u;
	}

	bool ComputeShader::on_execute()
	{
		if (false == bind_buffer_to_GPU_register()) {
			return false; 
		}
			

		//데이터 카운트가 하나라도 0일경우 계산 불가
		 if (false == (
			 mCB_ComputeShader.TotalDataCount.x && 
			 mCB_ComputeShader.TotalDataCount.y && 
			 mCB_ComputeShader.TotalDataCount.z))
		 {
			 unbind_buffer_from_GPU_register();
			 return false;
		 }
			
		
		//상수버퍼를 통해 데이터 수를 업로드
		static ConstBuffer* const pCB = RenderManager::get_inst().get_const_buffer(eCBType::ComputeShader);
		pCB->set_data(&mCB_ComputeShader);
		pCB->bind_buffer_to_GPU_register();

		//쉐이더 바인딩
		RenderManager::get_inst().Context()->CSSetShader(m_CS.Get(), nullptr, 0);
		RenderManager::get_inst().Context()->Dispatch(mCB_ComputeShader.NumGroup.x, mCB_ComputeShader.NumGroup.y, mCB_ComputeShader.NumGroup.z);

		//데이터 정리
		unbind_buffer_from_GPU_register();

		return true;
	}






	eResult ComputeShader::create_shader(const void* _pByteCode, size_t _ByteCodeSize)
	{
		eResult result = eResult::Fail_Create;

		if (SUCCEEDED(RenderManager::get_inst().Device()->CreateComputeShader(
			_pByteCode,
			_ByteCodeSize,
			nullptr,
			m_CS.ReleaseAndGetAddressOf()))
			)
		{
			result = eResult::Success;
		}

		return result;
	}
}
