#pragma once
#include "IResource.h"
#include "define_GPU.h"
#include "define_Struct.h"

namespace mh
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

	struct tIndexInfo
	{
		ComPtr<ID3D11Buffer>    IndexBuffer;

		struct Value
		{
			D3D11_BUFFER_DESC       tIBDesc;
			UINT				    IdxCount;
		} Val;

		std::vector<UINT>		IdxSysMem;
	};

	class FBXLoader;
	class StructBuffer;
	class Skeleton;
	struct tFBXContainer;
	class Mesh : public IResource 
	{
	public:
		Mesh();
		virtual ~Mesh();

		eResult CreateFromContainer(const tFBXContainer* _fbxContainer);

		virtual eResult Save(const std::fs::path& _filePath) override;										
		virtual eResult Load(const std::fs::path& _filePath) override;

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

		UINT GetSubsetCount() const { return (UINT)mIndexInfos.size(); }
		float GetBoundingSphereRad() const { return mBoundingSphereRadius; }

		void SetSkeleton(std::shared_ptr<Skeleton> _pSkeleton) { mSkeleton = _pSkeleton; }
		std::shared_ptr<Skeleton> GetSkeleton() const { return mSkeleton; }


	private:
		//버퍼를 만들지 않고 단순 데이터만 집어넣음
		bool SetVertexBufferData(const void* _data, size_t _dataStride, size_t _dataCount);
		bool SetIndexBufferData(const UINT* _data, size_t _dataCount);

		bool CreateVertexBuffer();
		bool CreateIndexBuffer();

		//void CreateAnimSBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;
		UINT mVertexByteStride;
		UINT mVertexCount;

		//다를 수 있는 구조체의 통합 저장을 위해 unsigned char 형태로 저장.
		std::vector<unsigned char> mVertexSysMem;

		//로컬 스페이스 상에서의 바운딩 sphere의 반지름.
		float mBoundingSphereRadius;
		
		std::vector<tIndexInfo>		mIndexInfos;

		//주소는 MeshData에서 관리
		std::shared_ptr<Skeleton> mSkeleton;
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
