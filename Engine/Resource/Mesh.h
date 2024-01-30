#pragma once
#include "iResource.h"


#include "../Util/Serialize/BinarySerializer.h"
#include "../GPU/CommonGPU.h"
#include "../CommonStruct.h"

namespace ehw
{
	struct VertexBase
	{
		float4 Pos;
	};

	struct Vertex2D : public VertexBase
	{
		float2 UV;
	};

	struct Vertex3D : public VertexBase
	{
		float2 UV;
		float3 Tangent;	//접선 벡터
		float3 Normal;	//법선 벡터
		float3 BiNormal;//종법선 벡터

		//Animation 가중치 및 인덱스
		float4 Weights;
		float4 Indices;
	};


	


	class FBXLoader;
	class StructBuffer;
	class Skeleton;
	struct tFBXContainer;
	class Mesh final 
		: public iResource 
		, Serializable<BinarySerializer>
	{
	public:
		Mesh();
		virtual ~Mesh();

		eResult CreateFromContainer(const tFBXContainer* _fbxContainer);

		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) override;

		virtual eResult Serialize(BinarySerializer& _ser) override;
		virtual eResult DeSerialize(BinarySerializer& _ser) override;


		template <typename Vertex>
		inline bool Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx);

		template <typename Vertex>
		inline bool CreateVertexBuffer(const std::vector<Vertex>& _vecVtx);
		bool CreateVertexBuffer(const void* _data, size_t _dataStride, size_t _dataCount);

		bool CreateIndexBuffer(const UINT* _data, size_t _dataCount);
		inline bool CreateIndexBuffer(const std::vector<UINT>& _indices);

		void BindBuffer(UINT _subSet = 0u) const;
		void Render(UINT _subSet = 0u) const;
		void RenderAllMeshes() const;
		
		void RenderInstanced(UINT _subSet, UINT _instanceCount) const;

		UINT GetSubsetCount() const { return (UINT)m_indexInfos.size(); }
		float GetBoundingSphereRad() const { return mBoundingSphereRadius; }

		void SetSkeleton(std::shared_ptr<Skeleton> _pSkeleton) { m_skeleton = _pSkeleton; }
		std::shared_ptr<Skeleton> GetSkeleton() const { return m_skeleton; }


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
		void SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount);

		//m_indexInfos.back()에 데이터를 넣음
		void SetIndexBufferData(const UINT* _data, size_t _dataCount);

		//실제 버퍼 생성은 이 함수. 실패시 m_vertexInfo를 초기화함
		bool CreateVertexBufferInternal();
		//Vector의 맨 뒤 데이터를 가지고 생성 시도. 실패시 m_IndexInfos의 맨 뒤 데이터를 빼버림.
		bool CreateIndexBufferInternal();
	private:
		tVertexInfo m_vertexInfo;
		std::vector<tIndexInfo>		m_indexInfos;

		//주소는 MeshData에서 관리
		std::shared_ptr<Skeleton>	m_skeleton;

		//로컬 스페이스 상에서의 바운딩 box의 최소, 최대값
		struct BoundingBoxMinMax
		{
			float3 Min;
			float3 Max;
		} mBoundingBoxMinMax;

		//로컬 스페이스 상에서의 바운딩 sphere의 반지름.
		float mBoundingSphereRadius;
	};


	template<typename Vertex>
	inline bool Mesh::Create(const std::vector<Vertex>& _vecVtx, const std::vector<uint>& _vecIdx)
	{
		bool bSuccess = false;
		if (CreateVertexBuffer<Vertex>(_vecVtx))
		{
			bSuccess = true;
		}

		if (CreateIndexBuffer((UINT*)_vecIdx.data(), _vecIdx.size()))
		{
			bSuccess = true;
		}
		
		return bSuccess;
	}
	template<typename Vertex>
	inline bool Mesh::CreateVertexBuffer(const std::vector<Vertex>& _vecVtx)
	{
		static_assert(std::is_base_of_v<VertexBase, Vertex>);
		return CreateVertexBuffer(static_cast<const void*>(_vecVtx.data()), sizeof(Vertex), _vecVtx.size());
	}
	inline bool Mesh::CreateIndexBuffer(const std::vector<UINT>& _indices)
	{
		return CreateIndexBuffer(static_cast<const UINT*>(_indices.data()), _indices.size());
	}

}
