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
#include "Engine/GPU/MultiRenderTarget.h"

#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Renderer/Renderer.h"
#include "Engine/Game/Component/Light/Light_3D.h"

namespace ehw
{
	Com_Camera::Com_Camera()
		: Component(Com_Camera::concrete_class_name)
		, m_camera_matrices{}
		, m_projectionType(eProjectionType::None)
		, m_isEnableCulling(true)
		, m_aspectRation(1.0f)
		, m_nearDistance(1.0f)
		, m_farDistance(50000.f)
		, m_scale(1.0f)
	{
		EnableLayerMasks();
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
		RenderManager::GetInst().sceneRenderAgent().Register_camera(this);
	}

	void Com_Camera::OnDisable()
	{
		RenderManager::GetInst().sceneRenderAgent().Unregister_camera(this);
	}

	void Com_Camera::render_gameobjects(const tRenderQueue& _render_queue)
	{
		m_layer_filtered_objects.clear();
		m_transform_data.clear();

		//출력할 레이어만 걸러낸다.
		for (GameObject* obj : _render_queue.objects_to_render) {
			if (true == m_layerMasks[obj->GetLayer()]) {
				m_layer_filtered_objects.push_back(obj);

				tTransform trdata{};
				trdata.World = obj->GetComponent<Transform>()->get_world_matrix();
				trdata.InverseWorld = trdata.World.Invert();
				trdata.WorldView = trdata.World * m_camera_matrices.view;
				trdata.WVP = trdata.WorldView * m_camera_matrices.projection;

				m_transform_data.push_back(trdata);
			}
		}

		//material에서 처리해야 하는 데이터도 등록시킨 후 GPU 연동
		_render_queue.material->set_data_to_buffer(m_layer_filtered_objects);
		_render_queue.material->bind_buffer_to_GPU();

		//게임오브젝트 수만큼 인스턴싱 그리기 명령
		_render_queue.mesh->render_instanced((UINT)m_layer_filtered_objects.size());

		m_layer_filtered_objects.clear();
		m_transform_data.clear();
	}

	void Com_Camera::render_lights()
	{
		RenderManager::GetInst().GetMultiRenderTarget(eMRTType::Light)->Bind();
		//나중에 컬링도 진행할것.
		for (int i = 0; i < LIGHT_TYPE_MAX; ++i) {
			Transform::clear_buffer_data();
			const auto& lights = Light_3D::get_lights_by_type(i);

			for (auto* l : lights) {
				if (true == m_layerMasks[l->gameObject()->GetLayer()]) {
					l->add_to_buffer();

					Transform* tr = l->gameObject()->GetComponent<Transform>();
					tr->add_to_buffer(m_camera_matrices.view, m_camera_matrices.projection);
				}
			}

			Transform::bind_buffer_to_GPU_register();
			Light_3D::render_lights(i);
		}
	}

	void Com_Camera::CreateViewMatrix()
	{
		auto tr = gameObject()->transform();

		//트랜스폼이 업데이트 되지 않았을 경우 자신도 업데이트 할 필요 없음
		if (false == tr->is_transform_updated())
		{
			return;
		}

		MATRIX world = tr->get_world_matrix();
		//뷰행렬 = 카메라 앞으로 월드행렬의 물체들을 끌어오는 작업.
		//도로 끌어오는 작업이므로 월드행렬에 배치했던 순서의 역순으로 작업을 해주면 된다.
		//이동과 회전을 원래대로 되돌리는 행렬은 특정 행렬을 다시 원래 상태로 돌리는 행렬이라고 볼 수 있다.
		//(로컬 위치) * (이동행렬) * (회전행렬) = (월드 위치)
		//(월드 위치) * (이동과 회전을 되돌리는 행렬) = (로컬 위치)
		//이건 역행렬의 정의와 일치한다.
		// I * X = X
		// X * X^(-1) = I
		//그러므로 카메라의 위치와 회전만큼 ?주는 행렬은 카메라의 위치행렬과 회전행렬의 역행렬을 곱해주면 된다는 뜻이다.
		//(R * T)^(-1) = T^(-1) * R^(-1)
		//역행렬을 곱하면 순서가 반대로 됨. 주의할 것
		//또한 역행렬은 행렬마다 만들어지는 방식이 다르다. 어떤 행렬은 역행렬이 존재하지 않을 수도 있다.
		//역행렬의 존재 여부는 판별식(행렬곱 Det)으로 가능하다.
		//의 경우는 역행렬이 다 존재하므로 사용할 필요는 없음.


		//1. 위치의 역행렬 -> 그냥 음수만큼 이동해주면됨
		//x축으로 a, y축으로 b, z축으로 c만큼 이동했다고 하면
		//x축으로 -a, y축으로 -b, z축으로 -c만큼 이동해주면 됨.
		/*
		1 0 0 0
		0 1 0 0
		0 0 1 0
		-a -b -c 1
		*/
		m_camera_matrices.view = MATRIX::CreateTranslation(-world.Translation());

		//2. 회전
		//회전은 이동과는 역행렬의 모습은 다르지만 쉽게 구할수 있다.
		//이는 직교행렬의 성질 덕분이다.
		//직교행렬은 각 행(또는 열)끼리 수직인 행렬을 말한다.
		//이런 직교행렬은 전치행렬이 자신의 역행렬이 되는 특징을 가지고 있다.
		//회전행렬은 직교행렬이고, 역행렬은 돌렸던 걸 다시 원상복귀시키는 행렬이므로 
		//회전행렬을 직교하면 현재 회전각의 반대 방향으로 돌릴 수 있게 된다.
		//const Vec3& vecRot = Transform().GetRelativeRot();
		//const XMVECTOR& vecQut = XMQuaternionRotationRollPitchYawFromVector(vecRot);
		//Matrix tempmat = Matrix::CreateFromQuaternion(vecQut);
		//m_matView *= tempmat.Transpose();
		world.Translation(Vector3::Zero);

		world.Right(world.Right().Normalize());
		world.Up(world.Up().Normalize());
		world.Forward(world.Forward().Normalize());
		world.Transpose();

		//3. transform 상수버퍼 구조체에 업데이트 -> 안함. 나중에 render때 일괄적으로 view 행렬과 proj 행렬을 곱할 예정.
		//g_matCam.matViewProj = m_matView;
		m_camera_matrices.view *= world;


		//방법 2: MATRIX CreateLookAt 함수 사용 -> 카메라의 월드 위치 기준으로
		world = tr->get_world_matrix();
		Vector3 target = Vector3::Transform(Vector3::UnitZ, world);
		m_camera_matrices.view = MATRIX::CreateLookAt(world.Translation(), target, world.Up());
		m_camera_matrices.inverse_view = m_camera_matrices.view.Invert();


		////===========
		////투영 -> 사실 이건 한번만 구해줘도 됨 -> Setting()으로 이동함
		////===========
		//
		////1. 투영 행렬 생성
		//const Vec2& Resolution = cDevice::GetInst()->GetRenderResolution();
		////m_matProj = XMMatrixOrthographicLH(Resolution.x, Resolution.y, 1.f, 10000.f);

		////1-1. 원근 투영행렬(체험용)
		//m_AspectRatio = Resolution.x / Resolution.y;
		//m_matProj = XMMatrixPerspectiveFovLH(0.5f * XM_PI, m_AspectRatio, 1.f, 10000.f);
		//
		////2. 업데이트

		
	}

	inline void Com_Camera::SetCullEnable(bool _bCullingEnable)
	{
		m_cullingAgent.reset();

		m_isEnableCulling = _bCullingEnable;
		if (m_isEnableCulling)
		{
			switch (m_projectionType)
			{
			case ehw::eProjectionType::Perspective:
				m_cullingAgent = std::make_unique<CullingAgent_Perspective>();
				break;
			case ehw::eProjectionType::Orthographic:
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
		uint2 resolution = RenderManager::GetInst().GetResolution();
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
		ConstBuffer* cb = RenderManager::GetInst().GetConstBuffer(eCBType::Camera);
		cb->SetData(&m_camera_matrices);
		cb->bind_buffer_to_GPU_register();
	}

	Com_Camera::CullingAgent::CullingAgent(const std::string_view key)
		: Entity(key)
	{
	}

	Com_Camera::CullingAgent_Orthographic::CullingAgent_Orthographic()
		: Com_Camera::CullingAgent(CullingAgent_Orthographic::concrete_class_name)
	{
	}
	Com_Camera::CullingAgent_Orthographic::~CullingAgent_Orthographic()
	{
	}
	Com_Camera::CullingAgent_Perspective::CullingAgent_Perspective()
		: Com_Camera::CullingAgent(CullingAgent_Perspective::concrete_class_name)
	{
	}
	Com_Camera::CullingAgent_Perspective::~CullingAgent_Perspective()
	{
	}

}
