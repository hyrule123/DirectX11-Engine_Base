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

        //이 세개 벡터를 보통 TBN 이라고 부름.
        float3 tangent;	//접선 벡터
        float3 binormal;//종법선 벡터
        float3 Normal;	//법선 벡터

        //Animation 가중치 및 인덱스
        uint4 Indices;
        float4 Weights;
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
            bool result = create_vertex_buffer(_vecVtx.data(), (UINT)sizeof(Vertex), (UINT)_vecVtx.size());

            if (std::is_base_of_v<VertexBase, Vertex> && result) {
                compute_bounding_box();
            }

            return result;
        }
        bool create_vertex_buffer(const void* _data, UINT _dataStride, UINT _dataCount);

        void IA_set_vertex_buffer();

        virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _resource_name) const;
        virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _resource_name);

        virtual eResult serialize_binary(BinarySerializer* _ser) const override;
        virtual eResult deserialize_binary(const BinarySerializer* _ser) override;

        bool is_saved() const { return m_is_saved; }

    private:
        void reset();
        void compute_bounding_box();
        bool create_vertex_buffer_internal();

    private:
        D3D11_BUFFER_DESC m_desc;

        //Vertex 구조체는 언제든지 달라질수 있음 -> 통합 저장을 위해 unsigned char(byte) 형태로 저장.
        std::vector<unsigned char> m_data;

        UINT m_data_stride;
        UINT m_data_count;

        ComPtr<ID3D11Buffer> m_buffer;

        //로컬 스페이스 상에서의 바운딩 sphere의 반지름.
        float m_bounding_sphere_radius;

        //로컬 스페이스 상에서의 바운딩 box의 최소, 최대값
        struct BoundingBoxMinMax
        {
            float3 Min;
            float3 Max;
        } m_bounding_box;

        //한번 저장했을 경우 true로 바뀐다.
        mutable bool m_is_saved;
    };
}


