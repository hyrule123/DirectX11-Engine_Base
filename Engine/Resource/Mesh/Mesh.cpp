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
#include "Engine/Resource/Mesh/VertexBuffer.h"
#include "Engine/Resource/Model3D/Skeleton.h"

#include "Engine/Game/GameObject.h"



#ifdef max
#undef max
#endif

namespace ehw
{
	Mesh::Mesh()
		: Resource(Mesh::concrete_name)
		, m_vertex_buffer{}
		, m_index_buffer{}
		, m_index_topology{}
		, m_index_buffer_desc{}
		, m_index_count{}
		, m_index_buffer_data{}
		, m_skeleton{}
		, m_render_queue{}
	{
		m_index_buffer_desc.CPUAccessFlags = 0;
		m_index_buffer_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		m_index_buffer_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
		m_index_buffer_desc.MiscFlags = 0;
		m_index_buffer_desc.StructureByteStride = 0;
	}

	Mesh::~Mesh()
	{
	}

	eResult Mesh::save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const
	{
		std::fs::path key_path = _key_path;
		key_path.replace_extension(strKey::path::extension::Mesh);
		return SaveFile_Binary(_baseDir / key_path);
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

		if (m_vertex_buffer) {
			ResourceManager<VertexBuffer>::GetInst().save(m_vertex_buffer.get());
			ser << m_vertex_buffer->get_keypath();
		}
		else {
			ser << "";
		}

		ser << m_index_topology;
		ser << m_index_buffer_desc;
		ser << m_index_count;
		ser << m_index_buffer_data;

		if (m_skeleton) {
			ser << m_skeleton->get_keypath();
		}
		else {
			ser << "";
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
		
		//vertex buffer
		std::string keypath; ser >> keypath;
		if (false == keypath.empty()) {
			m_vertex_buffer = ResourceManager<VertexBuffer>::GetInst().load(keypath);
		}

		ser >> m_index_topology;
		ser >> m_index_buffer_desc;
		ser >> m_index_count;
		ser >> m_index_buffer_data;

		//skeleton
		ser >> keypath;
		if (false == keypath.empty()) {
			ResourceManager<VertexBuffer>::GetInst().load(keypath);
		}

		return eResult::Success;
	}

	bool Mesh::create_index_buffer_internal(const UINT* _data, UINT _dataCount)
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		HRESULT result = RenderManager::GetInst().Device()->CreateBuffer(&m_index_buffer_desc, &subData, m_index_buffer.ReleaseAndGetAddressOf());

		return SUCCEEDED(result);
	}

	bool Mesh::create(const void* _p_vtx, UINT _byte_stride, UINT _data_count, const std::vector<UINT>& _vecIdx)
	{
		m_vertex_buffer = std::make_shared<VertexBuffer>();
		if (false == m_vertex_buffer->create_vertex_buffer(_p_vtx, _byte_stride, _data_count)) {
			m_vertex_buffer = nullptr;
			ASSERT_DEBUG(false, "정점 생성 실패");
			return false;
		}

		m_index_buffer_data = _vecIdx;

		return create_index_buffer(m_index_buffer_data.data(), (UINT)m_index_buffer_data.size());
	}

	bool Mesh::create_index_buffer(const UINT* _data, UINT _dataCount)
	{
		if (nullptr == _data || 0 == _dataCount)
		{
			ASSERT_DEBUG(false, "인덱스가 저장된 포인터가 nullptr입니다.");
			return false;
		}

		//데이터 개수와 총 바이트 수를 기록
		m_index_count = _dataCount;
		m_index_buffer_desc.ByteWidth = (UINT)sizeof(UINT) * _dataCount;

		//인덱스 버퍼 데이터 복사
		m_index_buffer_data.clear();
		m_index_buffer_data.resize(_dataCount);
		memcpy(m_index_buffer_data.data(), _data, m_index_buffer_desc.ByteWidth);

		return create_index_buffer_internal(_data, _dataCount);
	}

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

		context->IASetPrimitiveTopology(m_index_topology);
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
		if (nullptr == _fbxContainer) {
			return eResult::Fail_Nullptr;
		}

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


