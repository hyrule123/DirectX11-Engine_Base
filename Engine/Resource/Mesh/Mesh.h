#pragma once
#include "Engine/Resource/Resource.h"

#include "Engine/Util/Serialize/Serializable.h"
#include "Engine/GPU/CommonGPU.h"

#include "Engine/Common.h"

#include "Engine/Resource/Mesh/VertexBuffer.h"

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

		template <typename Vertex>
		bool create(const std::vector<Vertex>& _vecVtx, const std::vector<UINT>& _vecIdx);

		bool create_index_buffer(const UINT* _data, size_t _dataCount);
		inline bool create_index_buffer(const std::vector<UINT>& _indices);

		void set_topology(D3D11_PRIMITIVE_TOPOLOGY _topology) { m_topology = _topology; }
		D3D11_PRIMITIVE_TOPOLOGY get_topology() { return m_topology; }

		UINT get_subset_count() const { return (UINT)m_indexInfos.size(); }
		float get_bounding_sphere_radius() const { return m_bounding_sphere_radius; }

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
		struct tVertexInfo
		{
			D3D11_BUFFER_DESC Desc;
			UINT ByteStride;
			UINT Count;
			//Vertex 구조체는 언제든지 달라질수 있음 -> 통합 저장을 위해 unsigned char(byte) 형태로 저장.
			std::vector<unsigned char> SysMem;

			ComPtr<ID3D11Buffer> Buffer;
		};

		//위 구조체에서 ByteStride만 빠짐.
		struct tIndexInfo
		{
			D3D11_BUFFER_DESC       Desc;
			UINT				    Count;
			std::vector<UINT>		SysMem;

			//위 데이터를 토대로 GPU에서 생성된 데이터 주소
			ComPtr<ID3D11Buffer>    Buffer;
		};

		//cf)아래 함수 나눠놓은 이유: 파일로부터 Load 할 때에는 아래 2개 함수만 호출함.
		//버퍼를 만들지 않고 데이터만 집어넣음
		void set_vertex_buffer_data(const void* _data, size_t _dataStride, size_t _dataCount);

		//m_indexInfos.back()에 데이터를 넣음
		void set_index_buffer_data(const UINT* _data, size_t _dataCount);

		//실제 버퍼 생성은 이 함수. 실패시 m_vertexInfo를 초기화함
		bool create_vertex_buffer_internal();
		//Vector의 맨 뒤 데이터를 가지고 생성 시도. 실패시 m_IndexInfos의 맨 뒤 데이터를 빼버림.
		bool create_index_buffer_internal();
	private:
		D3D11_PRIMITIVE_TOPOLOGY m_topology;

		std::shared_ptr<VertexBuffer> m_vertex_buffer;

		std::vector<tIndexInfo>		m_indexInfos;

		//주소는 MeshData에서 관리
		std::shared_ptr<Skeleton>	m_skeleton;

		std::unordered_map<Material*, std::vector<GameObject*>> m_render_queue;
	};

	template<typename Vertex>
	inline bool Mesh::create(const std::vector<Vertex>& _vecVtx, const std::vector<UINT>& _vecIdx)
	{
		m_vertex_buffer = std::make_shared(VertexBuffer);
		bool bSuccess = create_vertex_buffer<Vertex>(_vecVtx);
		bSuccess &= create_index_buffer(_vecIdx.data(), _vecIdx.size());
		return bSuccess;
	}

	inline bool Mesh::create_index_buffer(const std::vector<UINT>& _indices)
	{
		return create_index_buffer(static_cast<const UINT*>(_indices.data()), _indices.size());
	}

}
