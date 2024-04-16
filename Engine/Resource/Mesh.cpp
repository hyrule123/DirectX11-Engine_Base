#include "Engine/Resource/Mesh.h"

#include "Engine/CommonGlobalVar.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/GPUManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Util/define_Util.h"
#include "Engine/Util/Serialize/BinarySerializer.h"

#include "Engine/Resource/Model3D/FBXLoader.h"


#ifdef max
#undef max
#endif

namespace ehw
{
	Mesh::Mesh()
		: iResource(typeid(Mesh))
		, m_topology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, m_vertexInfo{}
		, m_indexInfos{}
		, m_skeleton{}
		, mBoundingBoxMinMax{}
		, mBoundingSphereRadius()
		//, mVertices{}
	{
		
		//최대값에는 초기값으로 부동소수점 최소값을, 최소값에는 초기값으로 부동소수점 최대값을 대입
		mBoundingBoxMinMax.Max = float3(-FLT_MAX);
		mBoundingBoxMinMax.Min = float3(FLT_MAX);
	}

	Mesh::~Mesh()
	{
	}

	eResult Mesh::Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const
	{
		std::fs::path filePath = _baseDir / _strKeyPath;
		filePath.replace_extension(strKey::path::extension::Mesh);
		return SaveFile_Binary(filePath);
	}

	eResult Mesh::Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath)
	{
		std::fs::path filePath = _baseDir / _strKeyPath;
		filePath.replace_extension(strKey::path::extension::Mesh);
		SetStrKey(_strKeyPath.string());
		return LoadFile_Binary(filePath);
	}

	eResult Mesh::Serialize_Binary(BinarySerializer* _ser) const
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		BinarySerializer& ser = *_ser;

		//토폴로지
		ser << m_topology;

		//m_vertexInfo: ID3D11Buffer 포인터를 제외하고 저장
		ser << m_vertexInfo.ByteStride;
		ser << m_vertexInfo.Count;
		ser << m_vertexInfo.Desc;
		ser << m_vertexInfo.SysMem;


		//m_indexInfos: m_vertexInfo와 같음. 대신 여긴 vector이므로 순회를 돌면서 저장한다.
		ser << m_indexInfos.size();
		for (size_t i = 0; i < m_indexInfos.size(); ++i)
		{
			ser << m_indexInfos[i].Count;
			ser << m_indexInfos[i].Desc;
			ser << m_indexInfos[i].SysMem;
		}

		return eResult::Success;
	}

	eResult Mesh::DeSerialize_Binary(const BinarySerializer* _ser)
	{
		if (nullptr == _ser)
		{
			ERROR_MESSAGE("Serializer가 nullptr 이었습니다.");
			return eResult::Fail_Nullptr;
		}

		const BinarySerializer& ser = *_ser;

		//토폴로지
		ser >> m_topology;

		//m_vertexInfo: ID3D11Buffer 포인터를 제외하고 저장
		ser >> m_vertexInfo.ByteStride;
		ser >> m_vertexInfo.Count;
		ser >> m_vertexInfo.Desc;
		ser >> m_vertexInfo.SysMem;

		//Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		if (false == CreateVertexBufferInternal())
		{
			ERROR_MESSAGE("버텍스 버퍼 로드 실패");
			return eResult::Fail;
		}


		//m_indexInfos: m_vertexInfo와 같음. 대신 여긴 vector이므로 순회를 돌면서 저장한다.
		size_t indexSize{};
		ser >> indexSize;

		//resize 대신 reserve 하는 이유: CreateIndexInternal() 함수에서 back()을 사용함.
		m_indexInfos.reserve(indexSize);
		for (size_t i = 0; i < indexSize; ++i)
		{
			tIndexInfo& indexInfo = m_indexInfos.emplace_back();

			ser >> indexInfo.Count;
			ser >> indexInfo.Desc;
			ser >> indexInfo.SysMem;

			if (false == CreateIndexBufferInternal())
			{
				m_indexInfos.clear();
				ERROR_MESSAGE("인덱스 버퍼 생성 중 에러가 발생했습니다.");
				return eResult::Fail_Create;
			}
		}

		return eResult::Success;
	}

	bool Mesh::CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		if (nullptr == _data)
			return false;

		SetVertexBufferData(_data, _dataStride, _dataCount);

		return CreateVertexBufferInternal();
	}

	void Mesh::SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount)
	{
		m_vertexInfo.ByteStride = (uint)_dataStride;
		m_vertexInfo.Count = (uint)_dataCount;


		// 버텍스 버퍼
		m_vertexInfo.Desc.ByteWidth = m_vertexInfo.ByteStride * m_vertexInfo.Count;
		m_vertexInfo.Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		m_vertexInfo.Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		m_vertexInfo.Desc.CPUAccessFlags = 0;

		//정점 데이터를 memcpy를 통해서 복사
		size_t byteSize = (size_t)m_vertexInfo.Desc.ByteWidth;
		m_vertexInfo.SysMem.resize(byteSize);

		memcpy(m_vertexInfo.SysMem.data(), _data, byteSize);
	}

	void Mesh::SetIndexBufferData(const UINT* _data, size_t _dataCount)
	{
		tIndexInfo& indexInfo = m_indexInfos.emplace_back(tIndexInfo{});

		indexInfo.Count = (UINT)_dataCount;
		indexInfo.Desc.ByteWidth = (UINT)(sizeof(UINT) * _dataCount);
		indexInfo.Desc.CPUAccessFlags = 0;
		indexInfo.Desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		indexInfo.Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		indexInfo.Desc.MiscFlags = 0;
		indexInfo.Desc.StructureByteStride = 0;

		//인덱스 버퍼 복사
		indexInfo.SysMem.resize(_dataCount);
		memcpy_s(indexInfo.SysMem.data(), indexInfo.Desc.ByteWidth, _data, indexInfo.Desc.ByteWidth);
	}

	bool Mesh::CreateVertexBufferInternal()
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = m_vertexInfo.SysMem.data();

		if (
			FAILED(GPUManager::Device()->CreateBuffer(&m_vertexInfo.Desc, &subData, m_vertexInfo.Buffer.GetAddressOf()))
			)
		{
			//실패시 내용 초기화
			m_vertexInfo = tVertexInfo{};
			return false;
		}

		float4 BoundingBoxMin = float4(FLT_MAX);
		float4 BoundingBoxMax = float4(-FLT_MAX);

		//성공시 Bounding Sphere의 반지름을 구해준다.
		//type이 없어진 1byte 단위로 구성되어 있으므로 다시 재구성하는 작업이 필요하다.
		for (size_t i = 0; i < m_vertexInfo.SysMem.size(); i += m_vertexInfo.ByteStride)
		{
			//메모리의 끝 위치보다는 작아야 함
			//ex)stride가 16이고 데이터 갯수가 1개일 경우 n + 16 - 1: 15를 넘어가면 안됨 
			ASSERT(m_vertexInfo.SysMem.size() > (i + sizeof(VertexBase) - 1u), "참조하는 정점 인덱스가 정점 최대 사이즈를 넘어섰습니다.");

			const VertexBase* vtx = reinterpret_cast<const VertexBase*>(&(m_vertexInfo.SysMem[i]));

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


		return true;
	}

	bool Mesh::CreateIndexBufferInternal()
	{
		tIndexInfo& indexInfo = m_indexInfos.back();
		if (nullptr != indexInfo.Buffer)
		{
			ERROR_MESSAGE("이미 생성된 Index 버퍼입니다.");
			return false;
		}

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = indexInfo.SysMem.data();

		if (
			FAILED(GPUManager::Device()->CreateBuffer(&indexInfo.Desc, &subData, indexInfo.Buffer.GetAddressOf()))
			)
		{
			m_indexInfos.pop_back();
			return false;
		}

		return true;
	}

	bool Mesh::CreateIndexBuffer(const UINT* _data, size_t _dataCount)
	{
		if (nullptr == _data)
		{
			return false;
		}

		SetIndexBufferData(_data, _dataCount);

		return CreateIndexBufferInternal();
	}

	//void Mesh::BindBuffers(UINT _subSet) const
	//{
	//	auto context = GPUManager::Context();

	//	// Input Assembeler 단계에 버텍스버퍼 정보 지정
	//	uint offset = 0;

	//	context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
	//	context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//}

	void Mesh::Render(UINT _subSet) const
	{
		if ((UINT)m_indexInfos.size() <= _subSet)
		{
			return;
		}

		auto context = GPUManager::Context();

		context->IASetPrimitiveTopology(m_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
		context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(m_indexInfos[_subSet].Count, 0, 0);
	}

	void Mesh::RenderAllMeshes() const
	{
		auto context = GPUManager::Context();

		context->IASetPrimitiveTopology(m_topology);
		UINT size = (UINT)m_indexInfos.size();
		for (UINT i = 0; i < size; ++i)
		{
			// Input Assembeler 단계에 버텍스버퍼 정보 지정
			uint offset = 0;

			context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
			context->IASetIndexBuffer(m_indexInfos[i].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

			context->DrawIndexed(m_indexInfos[i].Count, 0, 0);
		}
	}
	
	void Mesh::RenderInstanced(UINT _subSet, UINT _instanceCount) const
	{
		if ((UINT)m_indexInfos.size() <= _subSet)
		{
			return;
		}

		auto context = GPUManager::Context();

		context->IASetPrimitiveTopology(m_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
		context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexedInstanced(m_indexInfos[_subSet].Count, _instanceCount, 0, 0, 0);
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


