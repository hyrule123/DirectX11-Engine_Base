#include "Engine/Resource/Shader/iComputeShader.h"


#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/PathManager.h"
#include "Engine/Manager/GPUManager.h"
#include "Engine/Manager/RenderManager.h"

#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/CommonGPU.h"


#include "Engine/Resource/define_Resource.h"

#include <fstream>

namespace ehw
{
	iComputeShader::iComputeShader(const std::type_info& _typeID, uint3 _threadsPerGroup)
		: iShader(_typeID)
		, m_CSBlob(nullptr)
		, m_CS(nullptr)
		, mCB_ComputeShader{ _threadsPerGroup,  }
	{
		//스레드가 하나라도 0이 들어있으면 안됨
		ASSERT(_threadsPerGroup.x && _threadsPerGroup.y && _threadsPerGroup.z, "컴퓨트쉐이더 Thread Axis 중 하나에 0이 들어가 있습니다.");
	}

	iComputeShader::~iComputeShader()
	{
	}

	eResult iComputeShader::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
	{
		return CreateByCSO(_baseDir / _strKeyPath);
	}

	eResult iComputeShader::CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName)
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

	eResult iComputeShader::CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		SetEngineDefaultRes(true);

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

		return CreateShader(pCode, DestSize);
	}

	eResult iComputeShader::CreateByCSO(const std::filesystem::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathManager::GetShaderCSOPath();
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

		return CreateShader(pCode, m_CSBlob->GetBufferSize());
	}

	void iComputeShader::OnExcute()
	{
		if (false == BindData())
			return;

		//데이터 카운트가 하나라도 0일경우 계산 불가
		 if (false == (
			 mCB_ComputeShader.TotalDataCount.x && 
			 mCB_ComputeShader.TotalDataCount.y && 
			 mCB_ComputeShader.TotalDataCount.z))
		 {
			 UnBindData();
			 return;
		 }
			
		
		//상수버퍼를 통해 데이터 수를 업로드
		static ConstBuffer* const pCB = RenderManager::GetConstBuffer(eCBType::ComputeShader);
		pCB->SetData(&mCB_ComputeShader);
		pCB->BindData();

		//쉐이더 바인딩
		GPUManager::GetInst().Context()->CSSetShader(m_CS.Get(), nullptr, 0);
		GPUManager::GetInst().Context()->Dispatch(mCB_ComputeShader.NumGroup.x, mCB_ComputeShader.NumGroup.y, mCB_ComputeShader.NumGroup.z);

		//데이터 정리
		UnBindData();
	}






	eResult iComputeShader::CreateShader(const void* _pByteCode, size_t _ByteCodeSize)
	{
		eResult result = eResult::Fail_Create;

		if (SUCCEEDED(GPUManager::GetInst().Device()->CreateComputeShader(
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
