#include "Engine/Manager/RenderManager.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh/Mesh.h"

#include "Engine/Resource/Shader/GraphicsShader.h"
#include "Engine/Resource/Shader/ComputeShaders/ParticleShader.h"
#include "Engine/Resource/Shader/ComputeShaders/Animation3D_ComputeShader.h"
#include "Engine/Resource/Texture.h"
#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/CompiledShaderHeader/DefaultShaders.h"

#include "Engine/GPU/CommonGPU.h"
#include "Engine/GPU/ConstBuffer.h"

#include "Engine/Resource/Material/Deffered_Mtrl.h"

namespace ehw {
	void RenderManager::LoadDefaultMesh()
	{
#pragma region POINT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};
			std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
			pointMesh->set_engine_default_res(true);

			std::vector<uint> pointIndex = { 0 };
			bool result = false;

			std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>();
			
			if (false == vb->create_vertex_buffer(VecVtx2D)) {
				ASSERT(false, "point vertex 생성 실패");
			}
			result &= pointMesh->create_index_buffer(pointIndex);
			ASSERT(result, "point mesh 생성 실패");
			pointMesh->set_topology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::PointMesh, pointMesh);
		}

#pragma endregion
#pragma region RECT MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			Vertex2D vtx2d = {};

			//RECT
			vtx2d.position = float4(-0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(0.5f, 0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(-0.5f, -0.5f, 0.0f, 1.0f);
			vtx2d.UV = float2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			// Crate Mesh
			std::shared_ptr<Mesh> RectMesh = std::make_shared<Mesh>();
			RectMesh->set_engine_default_res(true);
			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::RectMesh, RectMesh);
			std::vector<uint> indices = { 0u, 1u, 2u, 0u, 2u, 3u, 0u };
			
			ASSERT(RectMesh->create(VecVtx2D, indices), "RectMesh 생성 실패");

			VecVtx2D.clear();

#pragma endregion
#pragma region DEBUG RECTMESH

			vtx2d.position = float4(-0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(0.f, 0.f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(0.5f, 0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(1.0f, 0.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(1.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			vtx2d.position = float4(-0.5f, -0.5f, -0.00001f, 1.0f);
			vtx2d.UV = float2(0.0f, 1.0f);
			VecVtx2D.push_back(vtx2d);
			vtx2d = Vertex2D{};

			// CreateBuffer Mesh
			std::shared_ptr<Mesh> debugmesh = std::make_shared<Mesh>();
			debugmesh->set_engine_default_res(true);

			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::DebugRectMesh, debugmesh);
			ASSERT(debugmesh->create(VecVtx2D, indices), "debugMesh 생성 실패");
		}
#pragma endregion

#pragma region CIRCLE MESH
		{
			std::vector<Vertex2D> VecVtx2D;
			std::vector<uint>     VecIdx;

			Vertex2D center{};
			center.position = float4(0.0f, 0.0f, 0.f, 1.0f);
			center.UV = float2(0.5f, 0.5f);
			VecVtx2D.push_back(center);

			int iSlice = 40;
			float fRadius = 0.5f;
			float fTheta = DirectX::XM_2PI / (float)iSlice;

			Vertex2D vtx2d = {};
			for (int i = 0; i < iSlice; ++i)
			{
				vtx2d.position.x = fRadius * cosf(fTheta * i);
				vtx2d.position.y = fRadius * sinf(fTheta * i);

				//UV는 중점 기준으로 더하거나 빼는 방식으로 해준다.
				vtx2d.UV.x = 0.5f + vtx2d.position.x;
				vtx2d.UV.y = 0.5f - vtx2d.position.y;	//반대 방향

				VecVtx2D.push_back(vtx2d);

				//정점배열의 첫번째는 중심점이 들어가 있으므로 i + 1 == 현재 정점버퍼의 사이즈가 된다
				//인덱스는 시계 방향으로 삽입
				//마지막 인덱스는 따로 직접 삽입
				if (i == (iSlice - 1))
					continue;

				VecIdx.push_back(0);
				VecIdx.push_back(i + 2);
				VecIdx.push_back(i + 1);
			}

			VecIdx.push_back(0);
			VecIdx.push_back(1);
			VecIdx.push_back((uint)VecVtx2D.size() - 1u);

			// Crate Mesh
			std::shared_ptr<Mesh> circle_mesh = std::make_shared<Mesh>();
			circle_mesh->set_engine_default_res(true);
			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::CircleMesh, circle_mesh);

			ASSERT(circle_mesh->create(VecVtx2D, VecIdx), "circle mesh 생성 실패");
		}
#pragma endregion
#pragma region Cube Mesh
		{
			Vertex3D vtx3d;
			std::vector<Vertex3D> VecVtx3D;

			//24개 만드는 이유: UV 좌표가 다 다름
			VecVtx3D.reserve(24);

			// 윗면
			vtx3d.position = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 1.f, 0.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, -1.f, 0.f);
			vtx3d.tangent = float3(-1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(-1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, 1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(1.f, 0.f, 0.f);
			vtx3d.tangent = float3(0.0f, -1.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 0.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, 0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, 0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, 1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, -1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, 0.5f, -0.5f, 1.0f);;
			vtx3d.UV = float2(0.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, 0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 0.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(0.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			vtx3d.position = float4(-0.5f, -0.5f, -0.5f, 1.0f);
			vtx3d.UV = float2(1.f, 1.f);
			vtx3d.Normal = float3(0.f, 0.f, -1.f);
			vtx3d.tangent = float3(1.0f, 0.0f, 0.0f);
			vtx3d.binormal = float3(0.0f, 1.0f, 1.0f);
			VecVtx3D.push_back(vtx3d);
			vtx3d = {};

			std::vector<uint> indices;
			for (size_t i = 0; i < 6; i++)
			{
				indices.push_back(static_cast<int>(i) * 4);
				indices.push_back(static_cast<int>(i) * 4 + 1);
				indices.push_back(static_cast<int>(i) * 4 + 2);

				indices.push_back(static_cast<int>(i) * 4);
				indices.push_back(static_cast<int>(i) * 4 + 2);
				indices.push_back(static_cast<int>(i) * 4 + 3);
			}

			// Crate Mesh
			std::shared_ptr<Mesh> cubMesh = std::make_shared<Mesh>();
			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::CubeMesh, cubMesh);
			cubMesh->create(VecVtx3D, indices);
		}
#pragma endregion

#pragma region Debug Cube Mesh
		{
			VertexBase vertexBase{};
			std::vector<VertexBase> vertices;
			vertices.reserve(8);

			//정점정보 생성
			vertices.push_back(VertexBase{ float4(-0.5f, 0.5f, -0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(0.5f, 0.5f, -0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(0.5f, -0.5f, -0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(-0.5f, -0.5f, -0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(-0.5f, 0.5f, 0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(0.5f, 0.5f, 0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(0.5f, -0.5f, 0.5f, 1.f) });
			vertices.push_back(VertexBase{ float4(-0.5f, -0.5f, 0.5f, 1.f) });

			//인덱스 정보 생성
			std::vector<UINT> indices = {
				0, 1, 2,
				0, 2, 3,

				1, 5, 6,
				1, 6, 2,

				3, 2, 6,
				3, 6, 7,

				4, 0, 3,
				4, 3, 7,

				4, 5, 1,
				4, 1, 0,

				5, 4, 7,
				5, 7, 6
			};

			// Crate Mesh
			std::shared_ptr<Mesh> debugCubeMesh = std::make_shared<Mesh>();
			ASSERT(debugCubeMesh->create(vertices, indices), "debugCubeMesh 생성 실패");
			debugCubeMesh->set_engine_default_res(true);
			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::DebugCubeMesh, debugCubeMesh);
			//debugCubeMesh->set_topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		}
#pragma endregion //Debug Cube Mesh


#pragma region Sphere Mesh
		{
			Vertex3D vtx3d{};
			std::vector<Vertex3D> VecVtx3D;
			VecVtx3D.reserve(2000);

			float fRadius = 0.5f;

			// Top
			vtx3d.position = float4(0.0f, fRadius, 0.0f, 1.0f);
			vtx3d.UV = float2(0.5f, 0.f);
			vtx3d.Normal = float3(0.0f, fRadius, 0.0f);
			vtx3d.Normal.Normalize();
			vtx3d.tangent = float3(1.f, 0.f, 0.f);
			vtx3d.binormal = float3(0.f, 0.f, 1.f);
			VecVtx3D.push_back(vtx3d);

			// Body
			uint iStackCount = 40; // 가로 분할 개수
			uint iSliceCount = 40; // 세로 분할 개수

			float fStackAngle = DirectX::XM_PI / iStackCount;
			float fSliceAngle = DirectX::XM_2PI / iSliceCount;

			float fUVXStep = 1.f / (float)iSliceCount;
			float fUVYStep = 1.f / (float)iStackCount;

			for (uint i = 1; i < iStackCount; ++i)
			{
				float phi = i * fStackAngle;

				for (uint j = 0; j <= iSliceCount; ++j)
				{
					vtx3d = {};
					float theta = j * fSliceAngle;

					vtx3d.position = float4(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
						, fRadius * cosf(i * fStackAngle)
						, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle), 1.0f);
					vtx3d.UV = float2(fUVXStep * j, fUVYStep * i);
					vtx3d.Normal = float3(vtx3d.position);
					//v.Normal.Normalize();

					vtx3d.tangent.x = -fRadius * sinf(phi) * sinf(theta);
					vtx3d.tangent.y = 0.f;
					vtx3d.tangent.z = fRadius * sinf(phi) * cosf(theta);
					vtx3d.tangent.Normalize();

					vtx3d.tangent.Cross(vtx3d.Normal, vtx3d.binormal);
					vtx3d.binormal.Normalize();

					VecVtx3D.push_back(vtx3d);
					
				}
			}

			// Bottom
			vtx3d = {};
			vtx3d.position = float4(0.f, -fRadius, 0.f, 1.0f);
			vtx3d.UV = float2(0.5f, 1.f);
			vtx3d.Normal = float3(vtx3d.position.x, vtx3d.position.y, vtx3d.position.z);
			vtx3d.Normal.Normalize();

			vtx3d.tangent = float3(1.f, 0.f, 0.f);
			vtx3d.binormal = float3(0.f, 0.f, -1.f);
			VecVtx3D.push_back(vtx3d);

			// 인덱스
			// 북극점
			std::vector<uint> indices;
			indices.reserve(10000);
			for (uint i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(0);
				indices.push_back(i + 2);
				indices.push_back(i + 1);
			}

			// 몸통
			for (uint i = 0; i < iStackCount - 2; ++i)
			{
				for (uint j = 0; j < iSliceCount; ++j)
				{
					// + 
					// | \
					// +--+
					indices.push_back((iSliceCount + 1) * (i)+(j)+1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

					// +--+
					//  \ |
					//    +
					indices.push_back((iSliceCount + 1) * (i)+(j)+1);
					indices.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
					indices.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
				}
			}

			// 남극점
			uint iBottomIdx = (uint)VecVtx3D.size() - 1;
			for (uint i = 0; i < iSliceCount; ++i)
			{
				indices.push_back(iBottomIdx);
				indices.push_back(iBottomIdx - (i + 2));
				indices.push_back(iBottomIdx - (i + 1));
			}

			std::shared_ptr<Mesh> sphereMesh = std::make_shared<Mesh>();
			ResourceManager<Mesh>::GetInst().Insert(strKey::defaultRes::mesh::SphereMesh, sphereMesh);

			sphereMesh->create<Vertex3D>(VecVtx3D, indices);
		}


#pragma endregion
	}

	void RenderManager::LoadDefaultShader()
	{
#pragma region 2D LAYOUT
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc2D;
		D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

		LayoutDesc.AlignedByteOffset = 0;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "POSITION";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

		LayoutDesc.AlignedByteOffset = 16;
		LayoutDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TEXCOORD";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc2D.push_back(LayoutDesc);
		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
#pragma endregion


#pragma region DEFAULT TRIANGLE SHADER
		{
			std::shared_ptr<GraphicsShader> TriangleShader = std::make_shared<GraphicsShader>();
			TriangleShader->set_engine_default_res(true);
			TriangleShader->CreateByHeader(eGSStage::Vertex, VS_Triangle, sizeof(VS_Triangle));
			TriangleShader->CreateByHeader(eGSStage::Pixel, PS_Triangle, sizeof(PS_Triangle));
			TriangleShader->SetInputLayoutDesc(vecLayoutDesc2D);
			TriangleShader->CreateInputLayout();

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::RectShader, TriangleShader);
		}
#pragma endregion
#pragma region SPRITE SHADER
		{
			std::shared_ptr<GraphicsShader> spriteShader = std::make_shared<GraphicsShader>();
			spriteShader->set_engine_default_res(true);
			spriteShader->CreateByHeader(eGSStage::Vertex, VS_Sprite, sizeof(VS_Sprite));
			spriteShader->CreateByHeader(eGSStage::Pixel, PS_Sprite, sizeof(PS_Sprite));
			spriteShader->SetRSState(eRSType::SolidNone);
			spriteShader->SetInputLayoutDesc(vecLayoutDesc2D);
			spriteShader->CreateInputLayout();


			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::SpriteShader, spriteShader);
		}

#pragma endregion
#pragma region UI SHADER
		{
			std::shared_ptr<GraphicsShader> uiShader = std::make_shared<GraphicsShader>();
			uiShader->set_engine_default_res(true);
			uiShader->CreateByHeader(eGSStage::Vertex, VS_UserInterface, sizeof(VS_UserInterface));
			uiShader->CreateByHeader(eGSStage::Pixel, PS_UserInterface, sizeof(PS_UserInterface));
			uiShader->SetInputLayoutDesc(vecLayoutDesc2D);
			uiShader->CreateInputLayout();


			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::UIShader, uiShader);
		}

#pragma endregion
#pragma region GRID SHADER
		{
			std::shared_ptr<GraphicsShader> gridShader = std::make_shared<GraphicsShader>();
			gridShader->set_engine_default_res(true);
			gridShader->CreateByHeader(eGSStage::Vertex, VS_Grid, sizeof(VS_Grid));
			gridShader->CreateByHeader(eGSStage::Pixel, PS_Grid, sizeof(PS_Grid));
			gridShader->SetInputLayoutDesc(vecLayoutDesc2D);
			gridShader->CreateInputLayout();

			gridShader->SetRSState(eRSType::SolidNone);
			gridShader->SetDSState(eDSType::NoWrite);
			gridShader->SetBSState(eBSType::AlphaBlend);

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::GridShader, gridShader);
		}

#pragma endregion
#pragma region DEBUG SHADER
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> debugLayoutDesc;
			D3D11_INPUT_ELEMENT_DESC LayoutDesc{};

			LayoutDesc.AlignedByteOffset = 0;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "POSITION";
			LayoutDesc.SemanticIndex = 0;
			debugLayoutDesc.push_back(LayoutDesc);
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};

			std::shared_ptr<GraphicsShader> debugShader = std::make_shared<GraphicsShader>();
			debugShader->set_engine_default_res(true);
			debugShader->CreateByHeader(eGSStage::Vertex, VS_Debug, sizeof(VS_Debug));
			debugShader->CreateByHeader(eGSStage::Pixel, PS_Debug, sizeof(PS_Debug));

			debugShader->SetInputLayoutDesc(debugLayoutDesc);
			debugShader->CreateInputLayout();

			//debugShader->CreateBuffer(eShaderStage::Vertex, "DebugVS.hlsl", "main");
			//debugShader->CreateBuffer(eShaderStage::Pixel, "DebugPS.hlsl", "main");
			debugShader->SetRSState(eRSType::WireframeNone);
			debugShader->SetDSState(eDSType::NoWrite);
			debugShader->SetBSState(eBSType::AlphaBlend);

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::DebugShader, debugShader);
		}

#pragma endregion


#pragma region PARTICLE SHADER
		{
			std::shared_ptr<GraphicsShader> particleShader = std::make_shared<GraphicsShader>();
			particleShader->set_engine_default_res(true);

			particleShader->CreateByHeader(eGSStage::Vertex, VS_Particle, sizeof(VS_Particle));
			particleShader->CreateByHeader(eGSStage::Geometry, GS_Particle, sizeof(GS_Particle));
			particleShader->CreateByHeader(eGSStage::Pixel, PS_Particle, sizeof(PS_Particle));
			particleShader->SetInputLayoutDesc(vecLayoutDesc2D);
			particleShader->CreateInputLayout();


			particleShader->SetRSState(eRSType::SolidNone);
			particleShader->SetDSState(eDSType::NoWrite);
			particleShader->SetBSState(eBSType::AlphaBlend);

			//TODO: 파티클을 위한 토폴로지 설정 해줘야함
			//particleShader->set_topology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::ParticleShader, particleShader);

			std::shared_ptr<ParticleShader> particleCS = std::make_shared<ParticleShader>();
			particleCS->set_engine_default_res(true);
			ResourceManager<ComputeShader>::GetInst().Insert(strKey::defaultRes::shader::compute::ParticleCS, particleCS);
			particleCS->CreateByHeader(CS_Particle, sizeof(CS_Particle));
		}
#pragma endregion
#pragma region POST PROCESS SHADER
		{
			std::shared_ptr<GraphicsShader> postProcessShader = std::make_shared<GraphicsShader>();
			postProcessShader->set_engine_default_res(true);
			postProcessShader->CreateByHeader(eGSStage::Vertex, VS_PostProcess, sizeof(VS_PostProcess));
			postProcessShader->CreateByHeader(eGSStage::Pixel, PS_PostProcess, sizeof(PS_PostProcess));
			postProcessShader->SetInputLayoutDesc(vecLayoutDesc2D);
			postProcessShader->CreateInputLayout();

			postProcessShader->SetDSState(eDSType::NoWrite);
			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::PostProcessShader, postProcessShader);
		}
#pragma endregion

#pragma region 3D 기본 입력 레이아웃
		std::vector<D3D11_INPUT_ELEMENT_DESC> vecLayoutDesc3D = vecLayoutDesc2D;

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 24;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "TANGENT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 36;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BINORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 48;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "NORMAL";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 60;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDWEIGHT";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);

		LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
		LayoutDesc.AlignedByteOffset = 76;
		LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		LayoutDesc.InputSlot = 0;
		LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		LayoutDesc.SemanticName = "BLENDINDICES";
		LayoutDesc.SemanticIndex = 0;
		vecLayoutDesc3D.push_back(LayoutDesc);


#pragma endregion

#pragma region BASIC 3D
		{
			std::shared_ptr<GraphicsShader> basic3DShader = std::make_shared<GraphicsShader>();
			basic3DShader->set_engine_default_res(true);

			basic3DShader->CreateByHeader(eGSStage::Vertex, VS_Basic3D, sizeof(VS_Basic3D));
			basic3DShader->CreateByHeader(eGSStage::Pixel, PS_Basic3D, sizeof(PS_Basic3D));
			basic3DShader->SetInputLayoutDesc(vecLayoutDesc3D);
			basic3DShader->CreateInputLayout();

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::Basic3DShader, basic3DShader);
		}
#pragma endregion

#pragma region DEFFERD
		std::shared_ptr<GraphicsShader> defferedShader = std::make_shared<GraphicsShader>();
		defferedShader->set_engine_default_res(true);

		defferedShader->CreateByHeader(eGSStage::Vertex, VS_Deffered, sizeof(VS_Deffered));
		defferedShader->CreateByHeader(eGSStage::Pixel, PS_Deffered, sizeof(PS_Deffered));

		defferedShader->SetInputLayoutDesc(vecLayoutDesc3D);
		defferedShader->CreateInputLayout();

		ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::DefferedShader, defferedShader);

		//defferdShader->SetRSState();
#pragma endregion


#pragma region LIGHT
		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->CreateByHeader(eGSStage::Vertex, VS_LightDir, sizeof(VS_LightDir));
			lightShader->CreateByHeader(eGSStage::Pixel, PS_LightDir, sizeof(PS_LightDir));

			lightShader->SetInputLayoutDesc(vecLayoutDesc2D);
			lightShader->CreateInputLayout();

			lightShader->SetRSState(eRSType::SolidBack);
			lightShader->SetDSState(eDSType::None);
			lightShader->SetBSState(eBSType::OneOne);

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::LightDirShader, lightShader);
		}

		{
			std::shared_ptr<GraphicsShader> lightShader = std::make_shared<GraphicsShader>();
			lightShader->CreateByHeader(eGSStage::Vertex, VS_LightPoint, sizeof(VS_LightPoint));
			lightShader->CreateByHeader(eGSStage::Pixel, PS_LightPoint, sizeof(PS_LightPoint));

			lightShader->SetInputLayoutDesc(vecLayoutDesc3D);
			lightShader->CreateInputLayout();

			lightShader->SetRSState(eRSType::SolidFront);
			lightShader->SetDSState(eDSType::None);
			lightShader->SetBSState(eBSType::OneOne);

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::LightPointShader, lightShader);
		}
#pragma endregion

#pragma region MERGE
		{
			LayoutDesc = D3D11_INPUT_ELEMENT_DESC{};
			LayoutDesc.AlignedByteOffset = 0;
			LayoutDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			LayoutDesc.InputSlot = 0;
			LayoutDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			LayoutDesc.SemanticName = "POSITION";
			LayoutDesc.SemanticIndex = 0;

			std::shared_ptr<GraphicsShader> MergeShader = std::make_shared<GraphicsShader>();
			MergeShader->set_engine_default_res(true);
			MergeShader->CreateByHeader(eGSStage::Vertex, VS_Merge, sizeof(VS_Merge));
			MergeShader->CreateByHeader(eGSStage::Pixel, PS_Merge, sizeof(PS_Merge));

			MergeShader->AddInputLayoutDesc(LayoutDesc);
			MergeShader->CreateInputLayout();

			MergeShader->SetRSState(eRSType::SolidBack);
			MergeShader->SetDSState(eDSType::None);
			MergeShader->SetBSState(eBSType::Default);

			ResourceManager<GraphicsShader>::GetInst().Insert(strKey::defaultRes::shader::graphics::MergeShader, MergeShader);
		}
#pragma endregion


#pragma region ANIMATION 3D
		std::shared_ptr<Animation3D_ComputeShader> Anim3DShader = std::make_shared<Animation3D_ComputeShader>();
		Anim3DShader->set_engine_default_res(true);
		Anim3DShader->CreateByHeader(CS_Animation3D, sizeof(CS_Animation3D));

		ResourceManager<ComputeShader>::GetInst().Insert(strKey::defaultRes::shader::compute::Animation3D, Anim3DShader);
#pragma endregion
	}

	void RenderManager::CreateBuffer()
	{
#pragma region CONSTANT BUFFER
		m_constBuffers[(uint)eCBType::Global] = std::make_unique<ConstBuffer>((uint)eCBType::Global);
		m_constBuffers[(uint)eCBType::Global]->create(sizeof(tCB_Global));
		m_constBuffers[(uint)eCBType::Global]->SetPresetTargetStage(eShaderStageFlag::ALL);

		UpdateGlobalCBuffer();

		m_constBuffers[(uint)eCBType::Camera] = std::make_unique<ConstBuffer>((uint)eCBType::Camera);
		m_constBuffers[(uint)eCBType::Camera]->create<tCamera>();

		m_constBuffers[(uint)eCBType::ComputeShader] = std::make_unique<ConstBuffer>((uint)eCBType::ComputeShader);
		m_constBuffers[(uint)eCBType::ComputeShader]->create<tCB_ComputeShader>();
		m_constBuffers[(uint)eCBType::ComputeShader]->SetPresetTargetStage(eShaderStageFlag::Compute);

		m_constBuffers[(uint)eCBType::Material] = std::make_unique<ConstBuffer>((uint)eCBType::Material);
		m_constBuffers[(uint)eCBType::Material]->create(sizeof(tSharedMaterialData));

		m_constBuffers[(uint)eCBType::Animation2D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation2D);
		m_constBuffers[(uint)eCBType::Animation2D]->create(sizeof(tCB_Animation2D));

		m_constBuffers[(uint)eCBType::ParticleSystem] = std::make_unique<ConstBuffer>((uint)eCBType::ParticleSystem);
		m_constBuffers[(uint)eCBType::ParticleSystem]->create(sizeof(tCB_ParticleSystem));

		m_constBuffers[(uint)eCBType::Noise] = std::make_unique<ConstBuffer>((uint)eCBType::Noise);
		m_constBuffers[(uint)eCBType::Noise]->create(sizeof(tCB_Noise));

		m_constBuffers[(uint)eCBType::Animation3D] = std::make_unique<ConstBuffer>((uint)eCBType::Animation3D);
		m_constBuffers[(uint)eCBType::Animation3D]->create<tCB_Animation3D>();

#pragma endregion
	}

	void RenderManager::LoadDefaultTexture()
	{
#pragma region STATIC TEXTURE
		using namespace strKey::defaultRes;

		std::shared_ptr<Texture> pTex = nullptr;

		pTex = ResourceManager<Texture>::GetInst().load(texture::noise_01);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::GetInst().load(texture::noise_02);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::GetInst().load(texture::noise_03);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::GetInst().load(texture::BasicCube);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::GetInst().load(texture::BasicCubeNormal);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

		pTex = ResourceManager<Texture>::GetInst().load(texture::Brick);
		ASSERT(nullptr != pTex, "텍스처 로드 실패");

#pragma endregion

		//noise
		//std::shared_ptr<Texture> m_noiseTexture = std::make_shared<Texture>();
		//m_noiseTexture->CreateBuffer(RenderManager::GetInst().GetResolutionX(), RenderManager::GetInst().GetResolutionY(), DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE);
		//m_noiseTexture->BindDataSRV(GPU::Register::t::NoiseTexture, eShaderStageFlag::Pixel);
	}


	void RenderManager::CreateSamplerStates()
	{

#pragma region sampler state
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;


		RenderManager::GetInst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Point].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		RenderManager::GetInst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Linear].GetAddressOf()
		);

		samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_ANISOTROPIC;
		RenderManager::GetInst().Device()->CreateSamplerState
		(
			&samplerDesc
			, m_samplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf()
		);


		RenderManager::GetInst().Context()->PSSetSamplers((uint)eSamplerType::Point
			, 1, m_samplerStates[(uint)eSamplerType::Point].GetAddressOf());

		RenderManager::GetInst().Context()->PSSetSamplers((uint)eSamplerType::Linear
			, 1, m_samplerStates[(uint)eSamplerType::Linear].GetAddressOf());

		RenderManager::GetInst().Context()->PSSetSamplers((uint)eSamplerType::Anisotropic
			, 1, m_samplerStates[(uint)eSamplerType::Anisotropic].GetAddressOf());

#pragma endregion
	}

	void RenderManager::CreateRasterizerStates()
	{
#pragma region Rasterizer state
		D3D11_RASTERIZER_DESC rsDesc = {};
		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
		rsDesc.DepthClipEnable = TRUE;

		RenderManager::GetInst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRSType::SolidBack].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
		rsDesc.DepthClipEnable = TRUE;

		RenderManager::GetInst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRSType::SolidFront].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		RenderManager::GetInst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRSType::SolidNone].GetAddressOf());

		rsDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
		rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;

		RenderManager::GetInst().Device()->CreateRasterizerState(&rsDesc
			, m_rasterizerStates[(uint)eRSType::WireframeNone].GetAddressOf());
#pragma endregion
	}

	void RenderManager::CreateBlendStates()
	{

#pragma region Blend State
		//None
		m_blendStates[(uint)eBSType::Default] = nullptr;

		D3D11_BLEND_DESC bsDesc = {};
		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;
		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		RenderManager::GetInst().Device()->CreateBlendState(&bsDesc, m_blendStates[(uint)eBSType::AlphaBlend].GetAddressOf());

		bsDesc.AlphaToCoverageEnable = false;
		bsDesc.IndependentBlendEnable = false;

		bsDesc.RenderTarget[0].BlendEnable = true;
		bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		RenderManager::GetInst().Device()->CreateBlendState(&bsDesc, m_blendStates[(uint)eBSType::OneOne].GetAddressOf());

#pragma endregion
	}

	void RenderManager::CreateDepthStencilStates()
	{
#pragma region Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC dsDesc = {};
		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;
		RenderManager::GetInst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDSType::Less].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_GREATER;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
		dsDesc.StencilEnable = false;

		RenderManager::GetInst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDSType::Greater].GetAddressOf());

		dsDesc.DepthEnable = true;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		RenderManager::GetInst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDSType::NoWrite].GetAddressOf());

		dsDesc.DepthEnable = false;
		dsDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
		dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ZERO;
		dsDesc.StencilEnable = false;

		RenderManager::GetInst().Device()->CreateDepthStencilState(&dsDesc
			, m_depthStencilStates[(uint)eDSType::None].GetAddressOf());
#pragma endregion
	}


	void RenderManager::LoadDefaultMaterial()
	{
		using namespace strKey::defaultRes;
#pragma region DEFAULT
		std::shared_ptr<GraphicsShader> shader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::RectShader);
		std::shared_ptr<Material> RectMaterial = std::make_shared<Material>();
		RectMaterial->set_shader(shader);
		RectMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::RectMaterial, RectMaterial);
#pragma endregion

#pragma region SPRITE
		std::shared_ptr <Texture> spriteTexture = ResourceManager<Texture>::GetInst().Find(texture::DefaultSprite);
		std::shared_ptr<GraphicsShader> spriteShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::SpriteShader);
		std::shared_ptr<Material> spriteMaterial = std::make_shared<Material>();
		spriteMaterial->set_rendering_mode(eRenderingMode::forward_opaque);
		spriteMaterial->set_shader(spriteShader);
		spriteMaterial->set_texture(eTextureSlot::Albedo, spriteTexture);
		spriteMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::SpriteMaterial, spriteMaterial);
#pragma endregion

#pragma region UI
		std::shared_ptr<GraphicsShader> uiShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::UIShader);
		std::shared_ptr<Material> uiMaterial = std::make_shared<Material>();
		uiMaterial->set_rendering_mode(eRenderingMode::forward_opaque);
		uiMaterial->set_shader(uiShader);
		uiMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::UIMaterial, uiMaterial);
#pragma endregion

#pragma region GRID
		std::shared_ptr<GraphicsShader> gridShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::GridShader);
		std::shared_ptr<Material> gridMaterial = std::make_shared<Material>();
		gridMaterial->set_shader(gridShader);
		gridMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::GridMaterial, gridMaterial);
#pragma endregion

#pragma region DEBUG
		std::shared_ptr<GraphicsShader> debugShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::DebugShader);
		std::shared_ptr<Material> debugMaterial = std::make_shared<Material>();
		debugMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		debugMaterial->set_shader(debugShader);
		debugMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::DebugMaterial, debugMaterial);
#pragma endregion

#pragma region PARTICLE
		std::shared_ptr<GraphicsShader> particleShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::ParticleShader);
		std::shared_ptr<Material> particleMaterial = std::make_shared<Material>();
		particleMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		particleMaterial->set_shader(particleShader);
		particleMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::ParticleMaterial, particleMaterial);
#pragma endregion

#pragma region POSTPROCESS
		std::shared_ptr<GraphicsShader> postProcessShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::PostProcessShader);
		std::shared_ptr<Material> postProcessMaterial = std::make_shared<Material>();
		postProcessMaterial->set_rendering_mode(eRenderingMode::post_process);
		postProcessMaterial->set_shader(postProcessShader);
		postProcessMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::PostProcessMaterial, postProcessMaterial);
#pragma endregion

#pragma region BASIC3D
		std::shared_ptr<GraphicsShader> basic3DShader = ResourceManager<GraphicsShader>::GetInst().Find(shader::graphics::Basic3DShader);
		std::shared_ptr<Material> basic3DMaterial = std::make_shared<Material>();
		basic3DMaterial->set_rendering_mode(eRenderingMode::forward_transparent);
		basic3DMaterial->set_shader(basic3DShader);

		basic3DMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(material::Basic3DMaterial, basic3DMaterial);
#pragma endregion


#pragma region DEFFERD
		Deffered_Mtrl::load_shaders();
#pragma endregion

#pragma region LIGHT
		{
			std::shared_ptr<GraphicsShader> lightShader = ResourceManager<GraphicsShader>::GetInst().Find(strKey::defaultRes::shader::graphics::LightDirShader);
			std::shared_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->set_rendering_mode(eRenderingMode::NONE);
			lightMaterial->set_shader(lightShader);
			lightMaterial->set_engine_default_res(true);
			ResourceManager<Material>::GetInst().Insert(strKey::defaultRes::material::LightDirMaterial, lightMaterial);
		}

		{
			std::shared_ptr<GraphicsShader> LightPointShader = ResourceManager<GraphicsShader>::GetInst().Find(strKey::defaultRes::shader::graphics::LightPointShader);
			std::shared_ptr<Material> lightMaterial = std::make_shared<Material>();
			lightMaterial->set_rendering_mode(eRenderingMode::NONE);
			lightMaterial->set_shader(LightPointShader);
			lightMaterial->set_engine_default_res(true);

			ResourceManager<Material>::GetInst().Insert(strKey::defaultRes::material::LightPointMaterial, lightMaterial);
		}

#pragma endregion

#pragma region MERGE
		std::shared_ptr<GraphicsShader> mergeShader = ResourceManager<GraphicsShader>::GetInst().Find(strKey::defaultRes::shader::graphics::MergeShader);
		std::shared_ptr<Material> mergeMaterial = std::make_shared<Material>();
		mergeMaterial->set_rendering_mode(eRenderingMode::NONE);
		mergeMaterial->set_shader(mergeShader);
		mergeMaterial->set_engine_default_res(true);
		ResourceManager<Material>::GetInst().Insert(strKey::defaultRes::material::MergeMaterial, mergeMaterial);
#pragma endregion
	}
}
