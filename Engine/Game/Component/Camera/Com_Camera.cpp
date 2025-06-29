#include "Engine/Game/Component/Camera/Com_Camera.h"

#include "Engine/GameEngine.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/Scene.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/SceneManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Material/Material.h"
#include "Engine/Resource/Mesh/Mesh.h"


#include "Engine/GPU/ConstBuffer.h"
#include "Engine/GPU/StructBuffer.h"
#include "Engine/GPU/MultiRenderTarget.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Renderer/Renderer.h"
#include "Engine/Game/Component/Light/Light_3D.h"

namespace core
{
	Com_Camera::Com_Camera()
		: Super(Com_Camera::s_static_type_name, s_component_order)
		, m_camera_matrices{}
		, m_projectionType(eProjectionType::None)
		, m_isEnableCulling(true)
		, m_aspectRation(1.0f)
		, m_nearDistance(1.0f)
		, m_farDistance(50000.f)
		, m_scale(1.0f)
		, m_light_3D_volume_meshes{}
		, m_light_3D_materials{}
		, m_light_3D_const_buffer()
		, m_light_3D_instancing_buffer()
		, m_light_3D_instancing_datas{}
	{
		EnableLayerMasks();

		//////////////////////////////// LIGHT //////////////////////////////
		m_light_3D_volume_meshes[LIGHT_TYPE_DIRECTIONAL] = ResourceManager<Mesh>::get_inst().load(name::defaultRes::mesh::RectMesh);
		m_light_3D_materials[LIGHT_TYPE_DIRECTIONAL] = ResourceManager<Material>::get_inst().load(name::defaultRes::material::LightDirMaterial);

		m_light_3D_volume_meshes[LIGHT_TYPE_POINT] = ResourceManager<Mesh>::get_inst().load(name::defaultRes::mesh::SphereMesh);
		m_light_3D_materials[LIGHT_TYPE_POINT] = ResourceManager<Material>::get_inst().load(name::defaultRes::material::LightPointMaterial);

		m_light_3D_const_buffer = ResourceManager<ConstBuffer>::get_inst().find("light_3D_const_buffer");
		m_light_3D_instancing_buffer = ResourceManager<StructBuffer>::get_inst().find("light_3D_instancing_buffer");
	}

	Com_Camera::~Com_Camera()
	{
	}

	eResult Com_Camera::serialize_json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_Camera::deserialize_json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_Camera::final_update()
	{
		ASSERT(eProjectionType::None != m_projectionType, "카메라의 투영행렬 타입을 설정하지 않았습니다.");

		CreateViewMatrix();
	}

	void Com_Camera::OnEnable()
	{
		RenderManager::get_inst().sceneRenderAgent().Register_camera(this);
	}

	void Com_Camera::OnDisable()
	{
		RenderManager::get_inst().sceneRenderAgent().Unregister_camera(this);
	}

	void Com_Camera::render_gameobjects(const tRenderQueue& _render_queue)
	{
		m_layer_filtered_objects.clear();
		m_transform_data.clear();

		//출력할 레이어만 걸러낸다. 가장 기본적인 Transform 버퍼는 여기서 업로드 한다.
		for (const s_ptr<GameObject>& obj : _render_queue.objects_to_render) {
			if (true == m_layerMasks[obj->get_layer()]) {
				m_layer_filtered_objects.push_back(obj);

				tTransform trdata{};
				trdata.World = obj->get_component<Transform>()->get_world_matrix();
				trdata.InverseWorld = trdata.World.Invert();
				trdata.WorldView = trdata.World * m_camera_matrices.view;
				trdata.WVP = trdata.WorldView * m_camera_matrices.projection;

				m_transform_data.push_back(trdata);
			}
		}

		Transform::set_data_to_buffer(m_transform_data);
		Transform::bind_buffer_to_GPU_register();

		//material에서 처리해야 하는 데이터도 등록시킨 후 GPU 연동
		_render_queue.material->bind_shader();
		_render_queue.material->set_data_to_instancing_buffer(m_layer_filtered_objects);
		_render_queue.material->bind_instancing_buffer_to_GPU_register();

		//게임오브젝트 수만큼 인스턴싱 그리기 명령
		_render_queue.mesh->render_instanced((UINT)m_layer_filtered_objects.size());
	}

	void Com_Camera::render_lights_3D(eLightType _light_type, const std::vector<Light_3D*>& _lights)
	{
		m_transform_data.clear();
		std::vector<tLightAttribute>& datacont = m_light_3D_instancing_datas[(int)_light_type];
		datacont.clear();

		//나중에 컬링도 진행할것.
		for (auto* l : _lights) {
			s_ptr<GameObject> obj = l->get_owner();

			//레이어 마스크와 일치하는지 확인(bit mask)
			if (obj && true == m_layerMasks[obj->get_layer()]) {
				//transform 데이터 넣고
				tTransform trdata{};
				trdata.World = obj->get_component<Transform>()->get_world_matrix();
				trdata.InverseWorld = trdata.World.Invert();
				trdata.WorldView = trdata.World * m_camera_matrices.view;
				trdata.WVP = trdata.WorldView * m_camera_matrices.projection;

				m_transform_data.push_back(trdata);

				//light attribute 데이터 넣고
				datacont.push_back(l->get_light_3D_attribute());
			}
		}

		tLightCount light_count;
		light_count.count = (uint)datacont.size();

		//Transform 데이터 설정 및 바인딩
		Transform::set_data_to_buffer(m_transform_data);
		Transform::bind_buffer_to_GPU_register();

		//Light count를 저장하는 상수버퍼 바인딩
		m_light_3D_const_buffer->set_data(&light_count);
		m_light_3D_const_buffer->bind_buffer_to_GPU_register();

		//light 인스턴싱 데이터 바인딩
		m_light_3D_instancing_buffer->set_data(datacont.data(), light_count.count);
		m_light_3D_instancing_buffer->bind_buffer_as_SRV();

		//material 바인딩 후 mesh 렌더
		m_light_3D_materials[(int)_light_type]->bind_shader();
		m_light_3D_volume_meshes[(int)_light_type]->render_instanced(light_count.count);
	}

	void Com_Camera::CreateViewMatrix()
	{
		if (nullptr == get_owner())
		{
			DEBUG_MESSAGE("owner 게임오브젝트가 없음");
			return;
		}

		auto tr = get_owner()->get_component<Transform>();

		//트랜스폼이 업데이트 되지 않았을 경우 자신도 업데이트 할 필요 없음
		if (false == tr->is_transform_updated())
		{
			return;
		}

		
#pragma region 직접 뷰행렬 만드는 법
		//MATRIX world = tr->get_world_matrix();
		/*
		뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
		도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
		이동과 회전을 원래대로 되돌리는 행렬은 특정 행렬을 다시 원래 상태로 돌리는 행렬이라고 볼 수 있다.
		(로컬 위치) * (이동행렬) * (회전행렬) = (월드 위치)
		(월드 위치) * (이동과 회전을 되돌리는 행렬) = (로컬 위치)
		이건 역행렬의 정의와 일치한다.
		 I * X = X
		 X * X^(-1) = I
		그러므로 카메라의 위치와 회전만큼 ?주는 행렬은 카메라의 위치행렬과 회전행렬의 역행렬을 곱해주면 된다는 뜻이다.
		(R * T)^(-1) = T^(-1) * R^(-1)
		역행렬을 곱하면 순서가 반대로 됨. 주의할 것
		또한 역행렬은 행렬마다 만들어지는 방식이 다르다. 어떤 행렬은 역행렬이 존재하지 않을 수도 있다.
		역행렬의 존재 여부는 판별식(행렬곱 Det)으로 가능하다.
		의 경우는 역행렬이 다 존재하므로 사용할 필요는 없음.
		
		1. 위치의 역행렬 -> 그냥 음수만큼 이동해주면됨
		x축으로 a, y축으로 b, z축으로 c만큼 이동했다고 하면
		x축으로 -a, y축으로 -b, z축으로 -c만큼 이동해주면 됨.
		
		1 0 0 0
		0 1 0 0
		0 0 1 0
		-a -b -c 1
		*/
		//m_camera_matrices.view = MATRIX::CreateTranslation(-world.Translation());

		/*
		2. 회전
		회전은 이동과는 역행렬의 모습은 다르지만 쉽게 구할수 있다.
		이는 직교행렬의 성질 덕분이다.
		직교행렬은 각 행(또는 열)끼리 수직인 행렬을 말한다.
		이런 직교행렬은 전치행렬이 자신의 역행렬이 되는 특징을 가지고 있다.
		회전행렬은 직교행렬이고, 역행렬은 돌렸던 걸 다시 원상복귀시키는 행렬이므 
		회전행렬을 직교하면 현재 회전각의 반대 방향으로 돌릴 수 있게 된다.
		world.Translation(Vector3::Zero);
		world.Right(world.Right().Normalize());
		world.Up(world.Up().Normalize());
		world.Forward(world.Forward().Normalize());
		world.Transpose();
		*/

		/*
		*/
#pragma endregion
		//방법 2: MATRIX CreateLookAt 함수 사용 -> 카메라의 월드 위치 기준으로
		const MATRIX world = tr->get_world_matrix();
		
		//바로 눈앞에 있는 물체의 좌표를 구한다.
		//ex)카메라가 0, 0, 0에 있을 경우 0, 0, 1에 있는 물체의 좌표를 구한다.
		Vector3 target = Vector3::Transform(Vector3::UnitZ, world);
		m_camera_matrices.view = MATRIX::CreateLookAt(world.Translation(), target, world.Up());

		//view의 역행렬은 world
		m_camera_matrices.inverse_view = world;

		////===========
		////투영 -> 사실 해상도, near, far, 배율 등이 변경되었을 때 한번만 구해줘도 됨 -> Setting()으로 이동함
		////===========
		//
		////1. 투영 행렬 생성
		//const Vec2& Resolution = cDevice::get_inst()->GetRenderResolution();
		////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

		////1-1. 원근 투영행렬(체험용)
		//m_AspectRatio = Resolution.x / Resolution.y;
		//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		//	
	}

	void Com_Camera::SetCullEnable(bool _bCullingEnable)
	{
		m_cullingAgent.reset();

		m_isEnableCulling = _bCullingEnable;
		if (m_isEnableCulling)
		{
			switch (m_projectionType)
			{
			case core::eProjectionType::Perspective:
				m_cullingAgent = std::make_unique<CullingAgent_Perspective>();
				break;
			case core::eProjectionType::Orthographic:
				m_cullingAgent = std::make_unique<CullingAgent_Orthographic>();
				break;
			default:
				ASSERT(nullptr, "Projection 타입이 설정되지 않았습니다.");
				break;
			}
		}
	}

	void Com_Camera::CreateProjectionMatrix()
	{
		uint2 resolution = RenderManager::get_inst().GetResolution();
		CreateProjectionMatrix(resolution.x, resolution.y);
	}

	void Com_Camera::CreateProjectionMatrix(uint ResolutionX, uint ResolutionY)
	{
		m_cullingAgent.reset();

		float width = (float)ResolutionX * m_scale;
		float height = (float)ResolutionY * m_scale;

		m_aspectRation = width / height;

		switch (m_projectionType)
		{
		case eProjectionType::Perspective:
			m_camera_matrices.projection = MATRIX::CreatePerspectiveFieldOfViewLH
			(
				DirectX::XM_2PI / 6.0f
				, m_aspectRation
				, m_nearDistance
				, m_farDistance
			);


			break;
		case eProjectionType::Orthographic:
			m_camera_matrices.projection = MATRIX::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, m_nearDistance, m_farDistance);

			break;

		default:
			ASSERT(false, "Projection Type이 설정되지 않았습니다.");
			break;
		}

		//Projection Mode가 변하면 Culling Agent도 교체 필요 -> 한 번 호출해줘야 한다
		SetCullEnable(m_isEnableCulling);
	}

	void Com_Camera::SetScale(float _scale)
	{
		if (_scale < 0.f)
			return;

		m_scale = _scale;
		CreateProjectionMatrix();
	}

	void Com_Camera::TurnLayerMask(uint32 _layer, bool _enable)
	{
		m_layerMasks.set((uint32)_layer, _enable);
	}

	void Com_Camera::bind_data_to_GPU()
	{
		ConstBuffer* cb = RenderManager::get_inst().get_const_buffer(eCBType::Camera);
		cb->set_data(&m_camera_matrices);
		cb->bind_buffer_to_GPU_register();
	}

	Com_Camera::CullingAgent::CullingAgent(const std::string_view key)
		: Entity(key)
	{
	}

	Com_Camera::CullingAgent_Orthographic::CullingAgent_Orthographic()
		: Com_Camera::CullingAgent(CullingAgent_Orthographic::s_static_type_name)
	{
	}
	Com_Camera::CullingAgent_Orthographic::~CullingAgent_Orthographic()
	{
	}
	Com_Camera::CullingAgent_Perspective::CullingAgent_Perspective()
		: Com_Camera::CullingAgent(CullingAgent_Perspective::s_static_type_name)
	{
	}
	Com_Camera::CullingAgent_Perspective::~CullingAgent_Perspective()
	{
	}

}
