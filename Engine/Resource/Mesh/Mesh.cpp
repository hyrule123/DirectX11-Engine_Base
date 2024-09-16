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
		: Resource(Mesh::concrete_class_name)
		, m_vertex_buffer{}
		, m_index_buffer{}
		, m_index_topology{}
		, m_index_buffer_desc{}
		, m_index_count{}
		, m_index_buffer_data{}
		, m_skeleton{}
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
			ser << m_vertex_buffer->get_path_key();
		}
		else {
			ser << "";
		}

		ser << m_index_topology;
		ser << m_index_buffer_desc;
		ser << m_index_count;
		ser << m_index_buffer_data;

		if (m_skeleton) {
			ser << m_skeleton->get_path_key();
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

	void Mesh::render() const
	{
		auto context = RenderManager::GetInst().Context();

		context->IASetPrimitiveTopology(m_index_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		m_vertex_buffer->IA_set_vertex_buffer();
		context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(m_index_count, 0, 0);
	}
	
	void Mesh::render_instanced(UINT _instanceCount) const
	{
		auto context = RenderManager::GetInst().Context();

		context->IASetPrimitiveTopology(m_index_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		m_vertex_buffer->IA_set_vertex_buffer();

		context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexedInstanced(m_index_count, _instanceCount, 0, 0, 0);
	}
}


