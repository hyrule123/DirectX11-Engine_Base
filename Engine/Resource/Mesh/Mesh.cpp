#include "Engine/Resource/Mesh/Mesh.h"

#include "Engine/GlobalVariable.h"

#include "Engine/GPU/StructBuffer.h"

#include "Engine/Manager/ResourceManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/PathManager.h"

#include "Engine/Util/define_Util.h"
#include "Engine/Util/Serialize/BinarySerializer.h"

#include "Engine/Resource/Model3D/FBXLoader.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/Game/GameObject.h"



#ifdef max
#undef max
#endif

namespace ehw
{
	Mesh::Mesh()
		: Resource(Mesh::concrete_name)
		, m_topology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, m_vertexInfo{}
		, m_indexInfos{}
		, m_skeleton{}
		, m_bounding_box{}
		, m_bounding_sphere_radius()
		//, mVertices{}
	{
		
		//최대값에는 초기값으로 부동소수점 최소값을, 최소값에는 초기값으로 부동소수점 최대값을 대입
		m_bounding_box.Max = float3(-FLT_MAX);
		m_bounding_box.Min = float3(FLT_MAX);
	}

	Mesh::~Mesh()
	{
	}

	eResult Mesh::save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
	{
		std::fs::path filePath = _baseDir / _key_path;
		filePath.replace_extension(strKey::path::extension::Mesh);
		return SaveFile_Binary(filePath);
	}

	eResult Mesh::load(const std::fs::path& _baseDir, const std::fs::path& _key_path)
	{
		std::fs::path filePath = _baseDir / _key_path;
		filePath.replace_extension(strKey::path::extension::Mesh);
		return LoadFile_Binary(filePath);
	}

	eResult Mesh::serialize_binary(BinarySerializer* _ser) const
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

	eResult Mesh::deserialize_binary(const BinarySerializer* _ser)
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
		if (false == create_vertex_buffer_internal())
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

			if (false == create_index_buffer_internal())
			{
				m_indexInfos.clear();
				ERROR_MESSAGE("인덱스 버퍼 생성 중 에러가 발생했습니다.");
				return eResult::Fail_Create;
			}
		}

		return eResult::Success;
	}


	void Mesh::set_vertex_buffer_data(const void* _data, size_t _dataStride, size_t _dataCount)
	{

	}

	void Mesh::set_index_buffer_data(const UINT* _data, size_t _dataCount)
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

	bool Mesh::create_index_buffer_internal()
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
			FAILED(RenderManager::GetInst().Device()->CreateBuffer(&indexInfo.Desc, &subData, indexInfo.Buffer.GetAddressOf()))
			)
		{
			m_indexInfos.pop_back();
			return false;
		}

		return true;
	}

	bool Mesh::create_index_buffer(const UINT* _data, size_t _dataCount)
	{
		if (nullptr == _data)
		{
			return false;
		}

		set_index_buffer_data(_data, _dataCount);

		return create_index_buffer_internal();
	}

	//void Mesh::BindBuffers(UINT _subSet) const
	//{
	//	auto context = RenderManager::GetInst().Context();

	//	// Input Assembeler 단계에 버텍스버퍼 정보 지정
	//	uint offset = 0;

	//	context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
	//	context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	//}

	void Mesh::render_cached()
	{
		for (auto iter = m_render_queue.begin(); iter != m_render_queue.end(); ++iter) {
			iter->first->
		}
	}

	void Mesh::render(UINT _subSet) const
	{
		if ((UINT)m_indexInfos.size() <= _subSet)
		{
			return;
		}

		auto context = RenderManager::GetInst().Context();

		context->IASetPrimitiveTopology(m_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
		context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(m_indexInfos[_subSet].Count, 0, 0);
	}

	void Mesh::render_all_meshes() const
	{
		auto context = RenderManager::GetInst().Context();

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
	
	void Mesh::render_instanced(UINT _subSet, UINT _instanceCount) const
	{
		if ((UINT)m_indexInfos.size() <= _subSet)
		{
			return;
		}

		auto context = RenderManager::GetInst().Context();

		context->IASetPrimitiveTopology(m_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);
		context->IASetIndexBuffer(m_indexInfos[_subSet].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexedInstanced(m_indexInfos[_subSet].Count, _instanceCount, 0, 0, 0);
	}

	void Mesh::render_instanced_all(UINT _instance_count) const
	{
		auto context = RenderManager::GetInst().Context();

		context->IASetPrimitiveTopology(m_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		context->IASetVertexBuffers(0, 1, m_vertexInfo.Buffer.GetAddressOf(), &m_vertexInfo.ByteStride, &offset);

		for (size_t i = 0; i < m_indexInfos.size(); ++i) {
			context->IASetIndexBuffer(m_indexInfos[i].Buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexedInstanced(m_indexInfos[i].Count, _instance_count, 0, 0, 0);
		}
	}


	eResult Mesh::create_from_container(const tFBXContainer* _fbxContainer)
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
		create_vertex_buffer<Vertex3D>(vecVtx3d);

		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)_fbxContainer->vecIndexBuffers.size();
		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			create_index_buffer(_fbxContainer->vecIndexBuffers[i]);
		}

		return eResult::Success;
	}
}


