#pragma once
#include "Engine/Resource/Resource.h"
#include "Engine/GPU/CommonGPU.h"



namespace ehw {
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

    class VertexBuffer :
        public Resource
    {
        CLASS_NAME(VertexBuffer);

    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        //VertexBase를 상속받은 구조체만 가능
        //굳이 이렇게 한 이유: Bounding Box를 구해야 됨
        template <typename Vertex> requires std::is_base_of_v<VertexBase, Vertex>
        bool create_vertex_buffer(const std::vector<Vertex>& _vecVtx) {
            return create_vertex_buffer(static_cast<const void*>(_vecVtx.data()), (UINT)sizeof(Vertex), (UINT)_vecVtx.size());
        }
        bool create_vertex_buffer(const void* _data, UINT _dataStride, UINT _dataCount);

    private:
        void reset();

    private:
        D3D11_BUFFER_DESC m_desc;
        UINT m_byte_stride;
        UINT m_count;
        //Vertex 구조체는 언제든지 달라질수 있음 -> 통합 저장을 위해 unsigned char(byte) 형태로 저장.
        std::vector<unsigned char> m_data;

        ComPtr<ID3D11Buffer> m_buffer;

        //로컬 스페이스 상에서의 바운딩 sphere의 반지름.
        float m_bounding_sphere_radius;

        //로컬 스페이스 상에서의 바운딩 box의 최소, 최대값
        struct BoundingBoxMinMax
        {
            float3 Min;
            float3 Max;
        } m_bounding_box;
    };
}


