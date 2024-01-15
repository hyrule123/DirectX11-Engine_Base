
#include "Mesh.h"



#include "../CommonGlobalVar.h"

#include "../GPU/StructBuffer.h"

#include "../Manager/ResourceManager.h"
#include "../Manager/RenderManager.h"
#include "../Manager/GPUManager.h"
#include "../Manager/PathManager.h"

#include "../Util/define_Util.h"
#include "../Util/Serializer.h"


#include "Model3D/FBXLoader.h"


#ifdef max
#undef max
#endif

namespace ehw
{
	Mesh::Mesh()
		: iResource(typeid(Mesh))
		, mVertexBuffer{}
		, mVBDesc{}
		, mVertexByteStride{}
		, mVertexCount{}
		, mVertexSysMem()
		, mBoundingBoxMinMax{}
		, mBoundingSphereRadius()
		//, mVertices{}
		, mIndexInfos{}
		, mSkeleton{}
	{
		
		//최대값에는 초기값으로 부동소수점 최소값을, 최소값에는 초기값으로 부동소수점 최대값을 대입
		mBoundingBoxMinMax.Max = float3(-FLT_MAX);
		mBoundingBoxMinMax.Min = float3(FLT_MAX);
	}

	Mesh::~Mesh()
	{
	}

	eResult Mesh::Save(const std::fs::path& _pathFromBaseDir)
	{
		iResource::Save(_pathFromBaseDir);

		std::fs::path fullPath = ResourceManager<Mesh>::GetBaseDir() / _pathFromBaseDir;
		fullPath.replace_extension(strKey::path::extension::Mesh);

		std::ofstream ofs(fullPath, std::ios::binary);
		if (false == ofs.is_open())
		{
			return eResult::Fail_Open;
		}
		
		const std::string& str = "asdf";
		using deref = std::remove_const_t<std::remove_reference_t<const std::string&>>;
		//Key값 저장
		BinarySerializer ser{};
		ser << GetStrKey();
		

		//Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		//저장 필요 없음

		//D3D11_BUFFER_DESC mVBDesc;
		ser << mVBDesc;
		
		//UINT mVertexByteStride;
		
		Binary::SaveValue(ofs, mVertexByteStride);
		//ofs << mVertexByteStride;

		//UINT mVertexCount;
		Binary::SaveValue(ofs, mVertexCount);
		//ofs << mVertexCount;

		//std::vector<unsigned char> mVertexSysMem;
		Binary::SaveValueVector(ofs, mVertexSysMem);

		//std::vector<tIndexInfo>		mIndexInfos;
		Binary::SaveValue(ofs, mIndexInfos.size());
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			Binary::SaveValue(ofs, mIndexInfos[i].Val);
			Binary::SaveValueVector(ofs, mIndexInfos[i].IdxSysMem);
		}
		

		//StructBuffer* m_pBoneFrameData;   // 전체 본 프레임 정보(크기, 이동, 회전) (프레임 개수만큼)
		//StructBuffer* m_pBoneOffset;	  // 각 뼈의 offset 행렬(각 뼈의 위치를 되돌리는 행렬) (1행 짜리)
		//저장 필요 X
		ofs.close();

		return eResult::Success;
	}

	eResult Mesh::Load(const std::fs::path& _pathFromBaseDir)
	{
		iResource::Load(_pathFromBaseDir);

		std::fs::path fullPath = ResourceManager<Mesh>::GetBaseDir() / _pathFromBaseDir;
		fullPath.replace_extension(strKey::path::extension::Mesh);

		if (false == std::fs::exists(fullPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}


		std::ifstream ifs(fullPath, std::ios::binary);
		if (false == ifs.is_open())
			return eResult::Fail_Open;

		//Key값 가져오기
		std::string strKey;
		Binary::LoadStr(ifs, strKey);
		SetStrKey(strKey);

		//D3D11_BUFFER_DESC mVBDesc;
		Binary::LoadValue(ifs, mVBDesc);

		//UINT mVertexByteStride;
		Binary::LoadValue(ifs, mVertexByteStride);
		//ifs >> mVertexByteStride;

		//UINT mVertexCount;
		Binary::LoadValue(ifs, mVertexCount);
		//ifs >> mVertexCount;

		//std::vector<unsigned char> mVertexSysMem;
		Binary::LoadValueVector(ifs, mVertexSysMem);

		//Bounding Box 사이즈 계산		
		

		//Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		//버퍼 생성
		if (false == CreateVertexBuffer())
		{
			ERROR_MESSAGE("버텍스 버퍼 로드 실패");
			return eResult::Fail;
		}
			
		//std::vector<tIndexInfo>		mIndexInfos;
		{
			size_t size{};
			Binary::LoadValue(ifs, size);

			//매번 생성해서 집어넣음
			mIndexInfos.reserve(size);
			for (size_t i = 0; i < size; ++i)
			{
				tIndexInfo info{};
				Binary::LoadValue(ifs, info.Val);
				Binary::LoadValueVector(ifs, info.IdxSysMem);
				mIndexInfos.push_back(info);
				if (false == CreateIndexBuffer())
				{
					ERROR_MESSAGE("인덱스 버퍼 로드 실패");
					return eResult::Fail;
				}
			}
		}


		return eResult::Success;
	}

	bool Mesh::CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		if (nullptr == _data)
			return false;
		else if (false == SetVertexBufferData(_data, _dataStride, _dataCount))
			return false;
		

		return CreateVertexBuffer();
	}

	bool Mesh::SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		bool retVal = false;
		mVertexByteStride = (uint)_dataStride;
		mVertexCount = (uint)_dataCount;

		// 버텍스 버퍼
		mVBDesc.ByteWidth = mVertexByteStride * mVertexCount;
		mVBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		mVBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mVBDesc.CPUAccessFlags = 0;

		//정점 데이터를 memcpy를 통해서 복사
		size_t byteSize = (size_t)mVBDesc.ByteWidth;
		mVertexSysMem.resize(byteSize);
		if (0 == memcpy_s(mVertexSysMem.data(), byteSize, _data, byteSize))
			retVal = true;
		
		return retVal;
	}

	bool Mesh::SetIndexBufferData(const UINT* _data, size_t _dataCount)
	{
		bool result = false;

		tIndexInfo indexInfo = {};
		indexInfo.Val.IdxCount = (UINT)_dataCount;
		indexInfo.Val.tIBDesc.ByteWidth = (UINT)(sizeof(UINT) * _dataCount);
		indexInfo.Val.tIBDesc.CPUAccessFlags = 0;
		indexInfo.Val.tIBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		indexInfo.Val.tIBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		indexInfo.Val.tIBDesc.MiscFlags = 0;
		indexInfo.Val.tIBDesc.StructureByteStride = 0;

		//인덱스 버퍼 복사
		indexInfo.IdxSysMem.resize(_dataCount);
		if (0 == memcpy_s(indexInfo.IdxSysMem.data(), indexInfo.Val.tIBDesc.ByteWidth, _data, indexInfo.Val.tIBDesc.ByteWidth))
		{
			result = true;
			mIndexInfos.push_back(indexInfo);
		}

		return result;
	}

	bool Mesh::CreateVertexBuffer()
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = mVertexSysMem.data();

		bool Result = SUCCEEDED(GPUManager::Device()->CreateBuffer(&mVBDesc, &subData, mVertexBuffer.GetAddressOf()));

		if (false == Result)
		{
			mVertexBuffer = nullptr;
			mVBDesc = {};
			mVertexByteStride = 0u;
			mVertexCount = 0u;
		}

		float4 BoundingBoxMin = float4(FLT_MAX);
		float4 BoundingBoxMax = float4(-FLT_MAX);

		//성공시 Bounding Sphere의 반지름을 구해준다.
		//type이 없어진 1byte 단위로 구성되어 있으므로 다시 재구성하는 작업이 필요하다.
		for (size_t i = 0; i < mVertexSysMem.size(); i += mVertexByteStride)
		{
			//최대 size를 넘어가면 에러 발생시킴
			ASSERT(mVertexSysMem.size() > i + sizeof(VertexBase), "참조하는 정점 인덱스가 정점 최대 사이즈를 넘어섰습니다.");

			const VertexBase* vtx = reinterpret_cast<const VertexBase*>(&(mVertexSysMem[i]));

			//Bounding Box 계산
			BoundingBoxMin = float4::Min(BoundingBoxMin, vtx->Pos);
			BoundingBoxMax = float4::Max(BoundingBoxMax, vtx->Pos);


			//Bounding Sphere 계산
			//메쉬와 제일 먼 정점까지의 거리를 기록한다.
			//최적화를 위해서 일단 제곱근을 구하지 않고 먼저 계산한다.
			float lenSq = float3(vtx->Pos.x, vtx->Pos.y, vtx->Pos.z).LengthSquared();
			mBoundingSphereRadius = std::max<float>(mBoundingSphereRadius, lenSq);
		}
		//Bounding Box 입력
		mBoundingBoxMinMax.Min = float3(BoundingBoxMin);
		mBoundingBoxMinMax.Max = float3(BoundingBoxMax);

		//마지막에 sqrt 한번 해준다.
		mBoundingSphereRadius = std::sqrtf(mBoundingSphereRadius);


		return Result;
	}

	bool Mesh::CreateIndexBuffer()
	{
		bool result = false;
		tIndexInfo& indexInfo = mIndexInfos.back();

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = indexInfo.IdxSysMem.data();

		result = SUCCEEDED(GPUManager::Device()->CreateBuffer(&indexInfo.Val.tIBDesc, &subData, indexInfo.IndexBuffer.GetAddressOf()));

		if (false == result)
		{
			mIndexInfos.pop_back();
		}

		return result;
	}



	bool Mesh::CreateIndexBuffer(const UINT* _data, size_t _dataCount)
	{
		if (nullptr == _data)
			return false;
		else if (false == SetIndexBufferData(_data, _dataCount))
			return false;

		return CreateIndexBuffer();
	}

	void Mesh::BindBuffer(UINT _subSet) const
	{
		if ((UINT)mIndexInfos.size() <= _subSet)
			return;

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;
		GPUManager::Context()->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), &mVertexByteStride, &offset);
		GPUManager::Context()->IASetIndexBuffer(mIndexInfos[_subSet].IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}

	void Mesh::Render(UINT _subSet) const
	{
		GPUManager::Context()->DrawIndexed(mIndexInfos[_subSet].Val.IdxCount, 0, 0);
	}

	void Mesh::RenderAllMeshes() const
	{
		for (size_t i = 0; i < mIndexInfos.size(); ++i)
		{
			GPUManager::Context()->DrawIndexed(mIndexInfos[i].Val.IdxCount, 0, 0);
		}
	}
	
	void Mesh::RenderInstanced(UINT _subSet, UINT _instanceCount) const
	{
		GPUManager::Context()->DrawIndexedInstanced(mIndexInfos[_subSet].Val.IdxCount, _instanceCount, 0, 0, 0);
	}


	eResult Mesh::CreateFromContainer(const tFBXContainer* _fbxContainer)
	{
		ASSERT(_fbxContainer, "fbx 컨테이너가 nullptr 입니다.");
		if (nullptr == _fbxContainer)
			return eResult::Fail_Nullptr;

		UINT iVtxCount = (UINT)_fbxContainer->vecPosition.size();
		std::vector<Vertex3D> vecVtx3d(iVtxCount);
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			vecVtx3d[i].Pos = float4(_fbxContainer->vecPosition[i], 1.f);			
			vecVtx3d[i].UV = _fbxContainer->vecUV[i];
			vecVtx3d[i].Normal = _fbxContainer->vecNormal[i];
			vecVtx3d[i].Tangent = _fbxContainer->vecTangent[i];
			vecVtx3d[i].BiNormal = _fbxContainer->vecBinormal[i];

			if (_fbxContainer->bAnimation)
			{
				vecVtx3d[i].Weights = _fbxContainer->vecBlendWeight[i];
				vecVtx3d[i].Indices = _fbxContainer->vecBlendIndex[i];
			}
		}

		//std::shared_ptr<Mesh> pMesh = std::make_shared<Mesh>();
		CreateVertexBuffer<Vertex3D>(vecVtx3d);

		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)_fbxContainer->vecIndexBuffers.size();
		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			CreateIndexBuffer(_fbxContainer->vecIndexBuffers[i]);
		}

		return eResult::Success;
	}
}


