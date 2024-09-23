#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Util/Serialize/Serializable.h"
#include "Engine/GPU/CommonGPU.h"

#include "Engine/Common.h"

#include "Engine/Resource/Mesh/VertexBuffer.h"
//Vertex Buffer: 여러 Mesh에서 데이터 공유
//Index Buffer: Mesh마다 딱 하나씩 들고 있음

namespace ehw
{
	class GameObject;
	class Material;
	class FBXLoader;
	class StructBuffer;
	class Skeleton;
	struct tFBXContainer;

	BASE_RESOURCE(Mesh);
	class Mesh final 
		: public Resource 
		, Serializable_Binary
	{
		CLASS_NAME(Mesh);
		REGISTER_INSTANCE_DEFAULT(Mesh);
	public:
		Mesh();
		virtual ~Mesh();

		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const override;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name) override;

		virtual eResult serialize_binary(BinarySerializer* _ser) const override;
		virtual eResult deserialize_binary(const BinarySerializer* _ser) override;

		void set_vertex_buffer(const std::shared_ptr<VertexBuffer>& _buf) {
			m_vertex_buffer = _buf;
		}

		void set_topology(D3D11_PRIMITIVE_TOPOLOGY _topology) {
			m_index_topology = _topology;
		}

		template <typename Vertex> requires std::is_base_of_v<VertexBase, Vertex>
		bool create_vertex_buffer(const std::vector<Vertex>& _vertices) {
			m_vertex_buffer = std::make_shared<VertexBuffer>();
			return m_vertex_buffer->create_vertex_buffer(_vertices);
		}
		
		bool create_index_buffer(const std::vector<UINT>& _indices, D3D11_PRIMITIVE_TOPOLOGY _topology) {
			if (create_index_buffer(static_cast<const UINT*>(_indices.data()), (UINT)_indices.size())) {
				set_topology(_topology);
				return true;
			}
			return false;
		}

		template <typename Vertex> requires std::is_base_of_v<VertexBase, Vertex>
		bool create(const std::vector<Vertex>& _vertices, const std::vector<UINT>& _indices, D3D11_PRIMITIVE_TOPOLOGY _topology) {
			bool result = create_vertex_buffer(_vertices);
			result = result && create_index_buffer(_indices, _topology);
			return result;
		}

		void set_skeleton(const std::shared_ptr<Skeleton>& _pSkeleton) { m_skeleton = _pSkeleton; }
		std::shared_ptr<Skeleton> get_skeleton() const { return m_skeleton; }
		
		void render() const;
		void render_instanced(UINT _instanceCount) const;

	private:
		bool create_index_buffer(const UINT* _data, UINT _dataCount);
		bool create_index_buffer_internal(const UINT* _data, UINT _dataCount);
	private:
		std::shared_ptr<VertexBuffer> m_vertex_buffer;
		ComPtr<ID3D11Buffer>    m_index_buffer;
		
		//Index Buffer Data
		D3D11_BUFFER_DESC       m_index_buffer_desc;
		UINT				    m_index_count;
		std::vector<UINT>		m_index_buffer_data;
		////////////////////		

		D3D11_PRIMITIVE_TOPOLOGY m_index_topology;

		//주소는 MeshData에서 관리
		std::shared_ptr<Skeleton>	m_skeleton;
	};
}
