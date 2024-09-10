#include "Engine/Resource/Mesh/VertexBuffer.h"

#include "Engine/Manager/RenderManager.h"

namespace ehw {

	VertexBuffer::VertexBuffer()
		: Resource(VertexBuffer::concrete_name)
	{
		reset();
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	bool VertexBuffer::create_vertex_buffer(const void* _data, UINT _dataStride, UINT _dataCount)
	{
		if (nullptr == _data) {
			return false;
		}
		reset();

		m_byte_stride = _dataStride;
		m_count = _dataCount;

		// 버텍스 버퍼
		m_desc.ByteWidth = m_byte_stride * m_count;
		m_desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
		m_desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		m_desc.CPUAccessFlags = 0;

		//정점 데이터를 memcpy를 통해서 복사
		m_data.resize((size_t)m_desc.ByteWidth);

		memcpy(m_data.data(), _data, m_desc.ByteWidth);

		D3D11_SUBRESOURCE_DATA subData = {};
		subData.pSysMem = m_data.data();

		if (
			FAILED(RenderManager::GetInst().Device()->CreateBuffer(&m_desc, &subData, m_buffer.GetAddressOf()))
			)
		{
			//실패시 내용 초기화
			reset();
			return false;
		}

		float4 BoundingBoxMin = float4(FLT_MAX);
		float4 BoundingBoxMax = float4(-FLT_MAX);

		//성공시 Bounding Sphere의 반지름을 구해준다.
		//type이 없어진 1byte 단위로 구성되어 있으므로 다시 재구성하는 작업이 필요하다.
		for (size_t i = 0; i < m_data.size(); i += m_byte_stride)
		{
			//메모리의 끝 위치보다는 작아야 함
			//ex)stride가 16이고 데이터 갯수가 1개일 경우 n + 16 - 1: 15를 넘어가면 안됨 
			ASSERT(m_data.size() > (i + sizeof(VertexBase) - 1u), "참조하는 정점 인덱스가 정점 최대 사이즈를 넘어섰습니다.");

			const VertexBase* vtx = reinterpret_cast<const VertexBase*>(&(m_data[i]));

			//Bounding Box 계산
			BoundingBoxMin = float4::Min(BoundingBoxMin, vtx->position);
			BoundingBoxMax = float4::Max(BoundingBoxMax, vtx->position);


			//Bounding Sphere 계산
			//메쉬와 제일 먼 정점까지의 거리를 기록한다.
			//최적화를 위해서 일단 제곱근을 구하지 않고 먼저 계산한다.
			float lenSq = float3(vtx->position.x, vtx->position.y, vtx->position.z).LengthSquared();
			m_bounding_sphere_radius = std::max<float>(m_bounding_sphere_radius, lenSq);
		}
		//Bounding Box 입력
		m_bounding_box.Min = float3(BoundingBoxMin);
		m_bounding_box.Max = float3(BoundingBoxMax);

		//마지막에 sqrt 한번 해준다.
		m_bounding_sphere_radius = std::sqrtf(m_bounding_sphere_radius);

		return true;
	}

	void VertexBuffer::reset()
	{
		m_desc = {};
		m_byte_stride = 0;
		m_count = 0;
		m_data.clear();
		m_buffer = nullptr;

		m_bounding_sphere_radius = std::numeric_limits<float>::max();

		m_bounding_box.Min = float3(std::numeric_limits<float>::max());
		m_bounding_box.Max = float3(std::numeric_limits<float>::min());
	}

}
