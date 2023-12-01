#include "PCH_Engine.h"

#include "PathMgr.h"

#include "ComputeShader.h"
#include "GPUMgr.h"
#include "RenderMgr.h"
#include "ConstBuffer.h"

#include "define_GPU.h"
#include "define_Res.h"



namespace ehw
{
	ComputeShader::ComputeShader(uint3 _threadsPerGroup)
		: IShader(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mCB_ComputeShader{ _threadsPerGroup,  }
	{
		//스레드가 하나라도 0이 들어있으면 안됨
		ASSERT(_threadsPerGroup.x && _threadsPerGroup.y && _threadsPerGroup.z, "컴퓨트쉐이더 Thread Axis 중 하나에 0이 들어가 있습니다.");
	}

	ComputeShader::~ComputeShader()
	{
	}

	eResult ComputeShader::Load(const std::filesystem::path& _filePath)
	{
		std::fs::path fullPath = PathMgr::CreateFullPathToContent(_filePath, eResourceType::ComputeShader);
		return CreateByCSO(fullPath);
	}

	eResult ComputeShader::CreateByCompile(const std::filesystem::path& _FullPath, const std::string_view _funcName)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob = nullptr;

		if (FAILED(D3DCompileFromFile(
			_FullPath.wstring().c_str(), 
			nullptr, 
			D3D_COMPILE_STANDARD_FILE_INCLUDE, 
			std::string(_funcName).c_str(), 
			ehw::SHADER_VERSION::CS, 
			0, 
			0, 
			mCSBlob.GetAddressOf(), 
			mErrorBlob.GetAddressOf())))
		{
			std::string ErrMsg = "Failed to compile Compute GraphicsShader!\n\n";
			ErrMsg += "<Error Info>\n";
			ErrMsg += static_cast<const char*>(mErrorBlob->GetBufferPointer());
			ERROR_MESSAGE_A(ErrMsg.c_str());

			mErrorBlob = nullptr;

			return eResult::Fail_Compile;
		}


		return eResult::Success;
	}

	eResult ComputeShader::CreateByHeader(const unsigned char* _pByteCode, size_t _ByteCodeSize)
	{
		//헤더 형태로 만드는 쉐이더는 무조건 엔진 내부 기본 리소스라고 가정한다.
		SetEngineDefaultRes(true);

		HRESULT hr = D3DCreateBlob(_ByteCodeSize, mCSBlob.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			ERROR_MESSAGE_W(L"GraphicsShader를 저장할 Blob 생성에 실패했습니다.");

			return eResult::Fail_Create;
		}

		unsigned char* pCode = reinterpret_cast<unsigned char*>(mCSBlob->GetBufferPointer());
		//할당된 메모리 사이즈는 무조건 같아야 함
		size_t DestSize = mCSBlob->GetBufferSize();
		ASSERT(_ByteCodeSize == DestSize, "할당된 메모리와 쉐이더 byte code의 사이즈가 다릅니다.");

		//데이터 복사
		memcpy_s(pCode, DestSize, _pByteCode, _ByteCodeSize);

		return CreateShader(pCode, DestSize);
	}

	eResult ComputeShader::CreateByCSO(const std::filesystem::path& _FileName)
	{
		//CSO 파일이 있는 폴더에 접근
		std::filesystem::path shaderBinPath = PathMgr::GetShaderCSOPath();
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
		bool result = SUCCEEDED(D3DCreateBlob(sFile.tellg(), mCSBlob.GetAddressOf()));
		ASSERT(result, "쉐이더를 저장할 공간 할당에 실패했습니다.");
		

		//커서를 처음으로 돌린 후 파일을 읽어준다.
		sFile.seekg(0, std::ios_base::beg);
		sFile.read((char*)mCSBlob->GetBufferPointer(), mCSBlob->GetBufferSize());
		sFile.close();

		unsigned char* pCode = reinterpret_cast<unsigned char*>(mCSBlob->GetBufferPointer());

		return CreateShader(pCode, mCSBlob->GetBufferSize());
	}

	void ComputeShader::OnExcute()
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
		static ConstBuffer* const pCB = RenderMgr::GetConstBuffer(eCBType::ComputeShader);
		pCB->SetData(&mCB_ComputeShader);
		pCB->BindData();

		//쉐이더 바인딩
		GPUMgr::Context()->CSSetShader(mCS.Get(), nullptr, 0);
		GPUMgr::Context()->Dispatch(mCB_ComputeShader.NumGroup.x, mCB_ComputeShader.NumGroup.y, mCB_ComputeShader.NumGroup.z);

		//데이터 정리
		UnBindData();
	}






	eResult ComputeShader::CreateShader(const void* _pByteCode, size_t _ByteCodeSize)
	{
		eResult result = eResult::Fail_Create;

		if (SUCCEEDED(GPUMgr::Device()->CreateComputeShader(
			_pByteCode,
			_ByteCodeSize,
			nullptr,
			mCS.ReleaseAndGetAddressOf()))
			)
		{
			result = eResult::Success;
		}

		return result;
	}
}
