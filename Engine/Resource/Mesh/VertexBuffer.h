#pragma once
#include "Engine/Resource/Resource.h"
#include "Engine/GPU/CommonGPU.h"

#include "Engine/Util/Serialize/Serializable.h"


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

    class VertexBuffer 
        : public Resource
        , public Serializable_Binary
    {
        CLASS_NAME(VertexBuffer);

    public:
        VertexBuffer();
        virtual ~VertexBuffer();

        //VertexBase를 상속받은 구조체만 가능
        //굳이 이렇게 한 이유: Bounding Box를 구해야 됨
        template <typename Vertex>
        bool create_vertex_buffer(const std::vector<Vertex>& _vecVtx) {
            return create_vertex_buffer(static_cast<const void*>(_vecVtx.data()), (UINT)sizeof(Vertex), (UINT)_vecVtx.size());
        }
        bool create_vertex_buffer(const void* _data, UINT _dataStride, UINT _dataCount);

        void bind_buffer_to_GPU();

        virtual eResult serialize_binary(BinarySerializer* _ser) const override;
        virtual eResult deserialize_binary(const BinarySerializer* _ser) override;
        

    private:
        void reset();

    private:
        D3D11_BUFFER_DESC m_desc;

        

        //Vertex 구조체는 언제든지 달라질수 있음 -> 통합 저장을 위해 unsigned char(byte) 형태로 저장.
        std::vector<unsigned char> m_data;

        UINT m_byte_stride;
        UINT m_data_count;

        ComPtr<ID3D11Buffer> m_buffer;
    };
}


