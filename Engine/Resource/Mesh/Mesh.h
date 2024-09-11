#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Util/Serialize/Serializable.h"
#include "Engine/GPU/CommonGPU.h"

#include "Engine/Common.h"

//Vertex Buffer: 여러 Mesh에서 데이터 공유
//Index Buffer: Mesh마다 딱 하나씩 들고 있음

namespace ehw
{
	class VertexBuffer;
	class GameObject;
	class Material;
	class FBXLoader;
	class StructBuffer;
	class Skeleton;
	struct tFBXContainer;
	class Mesh final 
		: public Resource 
		, Serializable_Binary
	{
		CLASS_NAME(Mesh);

	public:
		Mesh();
		virtual ~Mesh();

		eResult create_from_container(const tFBXContainer* _fbxContainer);

		virtual eResult save(const std::fs::path& _baseDir, const std::fs::path& _key_path) const override;
		virtual eResult load(const std::fs::path& _baseDir, const std::fs::path& _key_path) override;

		virtual eResult serialize_binary(BinarySerializer* _ser) const override;
		virtual eResult deserialize_binary(const BinarySerializer* _ser) override;

		bool create(const void* _p_vtx, UINT _byte_stride, UINT _data_count, const std::vector<UINT>& _vecIdx);
		template <typename Vertex> requires std::is_base_of_v<VertexBase, Vertex>
		bool create(const std::vector<Vertex>& _vecVtx, const std::vector<UINT>& _vecIdx) {
			return create(_vecVtx.data(), (UINT)sizeof(Vertex), (UINT)_vecVtx.size(), _vecIdx);
		}

		bool create_index_buffer(const UINT* _data, UINT _dataCount);
		bool create_index_buffer(const std::vector<UINT>& _indices) {
			return create_index_buffer(static_cast<const UINT*>(_indices.data()), _indices.size());
		}

		void set_skeleton(const std::shared_ptr<Skeleton>& _pSkeleton) { m_skeleton = _pSkeleton; }
		std::shared_ptr<Skeleton> get_skeleton() const { return m_skeleton; }

		void enqueue_render(Material* _mtrl, GameObject* _obj) {
			if (_mtrl && _obj) { m_render_queue[_mtrl].push_back(_obj); }
		}
		void render_cached();
		
		void render(UINT _subSet = 0u) const;
		void render_all_meshes() const;
		void render_instanced(UINT _subSet, UINT _instanceCount) const;
		void render_instanced_all(UINT _instance_count) const;

	private:
		bool create_index_buffer_internal(const UINT* _data, UINT _dataCount);
	private:
		std::shared_ptr<VertexBuffer> m_vertex_buffer;
		ComPtr<ID3D11Buffer>    m_index_buffer;
		
		//Index Buffer Data
		D3D11_PRIMITIVE_TOPOLOGY m_index_topology;
		D3D11_BUFFER_DESC       m_index_buffer_desc;
		UINT				    m_index_count;
		std::vector<UINT>		m_index_buffer_data;
		////////////////////		

		//주소는 MeshData에서 관리
		std::shared_ptr<Skeleton>	m_skeleton;

		std::unordered_map<Material*, std::vector<GameObject*>> m_render_queue;
	};
}
