
#include "Mesh.h"

#include <Engine/Global.h>

#include <Engine/GPU/StructBuffer.h>

#include <Engine/Manager/ResourceManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/RenderManager.h>
#include <Engine/Manager/PathManager.h>

#include <Engine/Util/define_Util.h>
#include <Engine/Util/Serialize/BinarySerializer.h>

#include <Engine/Resource/Model3D/FBXLoader.h>
#include <Engine/Resource/Material/Material.h>
#include <Engine/Resource/Mesh/VertexBuffer.h>
#include <Engine/Resource/Model3D/Skeleton.h>

#include <Engine/Game/GameObject.h>



#ifdef max
#undef max
#endif

namespace core
{
	Mesh::Mesh()
		: Resource()
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

	eResult Mesh::save(const std::fs::path& _base_directory) const
	{
		return save_file_binary(_base_directory);
	}

	eResult Mesh::load(const std::fs::path& _base_directory)
	{
		return load_file_binary(_base_directory);
	}

	eResult Mesh::serialize_binary(BinarySerializer& _ser) const
	{
		eResult result = Super::serialize_binary(_ser);
		if (eResult_fail(result)) { return result; }

		//vertex buffer는 필수
		if (nullptr == m_vertex_buffer) {
			ERROR_MESSAGE("Vertex Buffer가 있어야 저장할 수 있습니다.");
			return eResult::Fail;
		}

		//공유 리소스이므로 한번 저장했으면 더이상 저장할 필요 없음
		if (false == m_vertex_buffer->is_saved()) {
			eResult result = ResourceManager<VertexBuffer>::get_inst().save(m_vertex_buffer);

			if (eResult_fail(result)) {
				ERROR_MESSAGE("Vertex Buffer 저장 실패.");
				return result;
			}
		}
		_ser << m_vertex_buffer->get_res_filename();

		_ser << m_index_topology;
		_ser << m_index_buffer_desc;
		_ser << m_index_count;
		_ser << m_index_buffer_data;

		//skeleton은 필수 X
		if (m_skeleton) {
			_ser << m_skeleton->get_res_filename();
		}
		else {
			_ser << "";
		}

		return eResult::Success;
	}

	eResult Mesh::deserialize_binary(const BinarySerializer& _ser)
	{
		eResult result = Super::deserialize_binary(_ser);
		if (eResult_fail(result)) { return result; }
		
		//vertex buffer
		std::string res_name; _ser >> res_name;
		m_vertex_buffer = ResourceManager<VertexBuffer>::get_inst().load(res_name);

		_ser >> m_index_topology;
		_ser >> m_index_buffer_desc;
		_ser >> m_index_count;
		_ser >> m_index_buffer_data;

		if (false == create_index_buffer_internal(m_index_buffer_data.data(), (UINT)m_index_buffer_data.size())) {
			ERROR_MESSAGE("인덱스 버퍼 생성에 실패했습니다.");
			return eResult::Fail;
		}

		//skeleton
		_ser >> res_name;
		if (false == res_name.empty()) {
			m_skeleton = ResourceManager<Skeleton>::get_inst().load(res_name);
		}

		return eResult::Success;
	}

	bool Mesh::create_index_buffer_internal(const UINT* _data, UINT _dataCount)
	{
		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = _data;

		HRESULT result = RenderManager::get_inst().get_device()->CreateBuffer(&m_index_buffer_desc, &subData, m_index_buffer.ReleaseAndGetAddressOf());

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
		auto context = RenderManager::get_inst().get_context();

		context->IASetPrimitiveTopology(m_index_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		uint offset = 0;

		m_vertex_buffer->IA_set_vertex_buffer();
		context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed(m_index_count, 0, 0);
	}
	
	void Mesh::render_instanced(UINT _instanceCount) const
	{
		auto context = RenderManager::get_inst().get_context();

		context->IASetPrimitiveTopology(m_index_topology);

		// Input Assembeler 단계에 버텍스버퍼 정보 지정
		m_vertex_buffer->IA_set_vertex_buffer();
		context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexedInstanced(m_index_count, _instanceCount, 0, 0, 0);
	}
}


