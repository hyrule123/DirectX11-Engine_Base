#include "Engine/Manager/Resourcemanagers.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh/Mesh.h"

namespace core {
	void ResourceManagers::load_default_meshes()
	{
#pragma region POINT MESH
		{
			std::vector<Vertex2D> vertices_2D = { {} };

			std::shared_ptr<Mesh> pointMesh = std::make_shared<Mesh>();
			pointMesh->set_engine_default_res(true);

			std::vector<uint> pointIndex = { 0 };

			std::shared_ptr<VertexBuffer> vb = std::make_shared<VertexBuffer>();

			if (false == vb->create_vertex_buffer(vertices_2D)) {
				ASSERT(false, "point vertex 생성 실패");
			}
			if (false == pointMesh->create_index_buffer(pointIndex, D3D11_PRIMITIVE_TOPOLOGY_POINTLIST)) {
				ASSERT(false, "point vertex 생성 실패");
			}

			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::PointMesh, pointMesh);
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
			std::shared_ptr<Mesh> rect_mesh = std::make_shared<Mesh>();
			rect_mesh->set_engine_default_res(true);

			std::vector<uint> indices = { 0u, 1u, 2u, 0u, 2u, 3u, 0u };

			ASSERT(rect_mesh->create(VecVtx2D, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "rect_mesh 생성 실패");

			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::RectMesh, rect_mesh);

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

			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::DebugRectMesh, debugmesh);
			ASSERT(debugmesh->create(VecVtx2D, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "debugMesh 생성 실패");
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
			circle_mesh->set_topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			ASSERT(circle_mesh->create(VecVtx2D, VecIdx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "circle mesh 생성 실패");

			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::CircleMesh, circle_mesh);


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
			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::CubeMesh, cubMesh);
			ASSERT(cubMesh->create(VecVtx3D, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "cube mesh 생성 실패");
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
			ASSERT(debugCubeMesh->create(vertices, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "debugCubeMesh 생성 실패");
			debugCubeMesh->set_engine_default_res(true);
			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::DebugCubeMesh, debugCubeMesh);
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
			ASSERT(sphereMesh->create<Vertex3D>(VecVtx3D, indices, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST), "sphere mesh 생성 실패");
			ResourceManager<Mesh>::get_inst().insert(name::defaultRes::mesh::SphereMesh, sphereMesh);
		}


#pragma endregion
	}
}
