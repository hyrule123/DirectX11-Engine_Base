#include "Engine/Game/Component/Camera/Com_Camera.h"

#include "Engine/GameEngine.h"

#include "Engine/Game/GameObject.h"
#include "Engine/Game/iScene.h"

#include "Engine/Manager/RenderManager.h"
#include "Engine/Manager/SceneManager.h"
#include "Engine/Manager/GPUManager.h"
#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Material.h"

#include "Engine/GPU/MultiRenderTarget.h"

#include "Engine/Game/Component/Transform/Com_Transform.h"
#include "Engine/Game/Component/Renderer/iRenderer.h"
#include "Engine/Game/Component/Light/Com_Light3D.h"

namespace ehw
{
	MATRIX Com_Camera::gView = MATRIX::Identity;
	MATRIX Com_Camera::gViewInverse = MATRIX::Identity;
	MATRIX Com_Camera::gProjection = MATRIX::Identity;

	Com_Camera::Com_Camera()
		: mView()
		, mViewInverse()
		, mProjection()
		, mProjType(eProjectionType::None)
		, mbCullEnable(true)
		, mAspectRatio(1.0f)
		, mNear(1.0f)
		, mFar(50000.f)
		, m_scale(1.0f)
	{
		EnableLayerMasks();
	}

	Com_Camera::~Com_Camera()
	{
	}

	eResult Com_Camera::Serialize_Json(JsonSerializer* _ser) const
	{
		return eResult();
	}

	eResult Com_Camera::DeSerialize_Json(const JsonSerializer* _ser)
	{
		return eResult();
	}

	void Com_Camera::OnEnable()
	{

	}

	void Com_Camera::Awake()
	{
	}

	void Com_Camera::Update()
	{
	}

	void Com_Camera::FinalUpdate()
	{
		ASSERT(eProjectionType::None != mProjType, "카메라의 투영행렬 타입을 설정하지 않았습니다.");

		CreateViewMatrix();

		RegisterCameraInRenderer();
	}

	void Com_Camera::RenderCamera()
	{
		gView = mView;
		gViewInverse = mView.Invert();
		gProjection = mProjection;

		SortGameObjects();

		//deffered opaque render
		RenderManager::GetMultiRenderTarget(eMRTType::Deffered)->Bind();
		RenderDeffered();

		//// deffered light 
		RenderManager::GetMultiRenderTarget(eMRTType::Light)->Bind();
		// 여러개의 모든 빛을 미리 한장의 텍스처에다가 계산을 해두고
		// 붙여버리자

		const auto& Lights = RenderManager::GetLights();
		for (size_t i = 0; i < Lights.size(); ++i)
		{
			if (false == Lights[i].expired())
			{
				Lights[i].lock()->Render();
			}
		}

		// Forward render
		RenderManager::GetMultiRenderTarget(eMRTType::Swapchain)->Bind();


		//// defferd + swapchain merge
		std::shared_ptr<Material> mergeMaterial = ResourceManager<Material>::Find(strKey::defaultRes::material::MergeMaterial);
		std::shared_ptr<Mesh> rectMesh = ResourceManager<Mesh>::Find(strKey::defaultRes::mesh::RectMesh);
		rectMesh->BindBuffer();
		mergeMaterial->BindData();
		rectMesh->Render();


		RenderForwardOpaque();
		RenderCutout();
		RenderTransparent();
		RenderPostProcess();
	}

	void Com_Camera::CreateViewMatrix()
	{
		auto tr = GetOwner()->Transform();

		//트랜스폼이 업데이트 되지 않았을 경우 자신도 업데이트 할 필요 없음
		if (false == tr->IsTransformUpdated())
		{
			return;
		}

		MATRIX world = tr->GetWorldMatrix();
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
		mView = MATRIX::CreateTranslation(-world.Translation());

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
		mView *= world;


		//방법 2: MATRIX CreateLookAt 함수 사용 -> 카메라의 월드 위치 기준으로
		world = tr->GetWorldMatrix();
		Vector3 target = Vector3::Transform(Vector3::UnitZ, world);
		mView = MATRIX::CreateLookAt(world.Translation(), target, world.Up());



		////===========
		////투영 -> 사실 이건 한번만 구해줘도 됨 -> Init()으로 이동함
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

		mViewInverse = mView.Invert();
	}

	inline void Com_Camera::SetCullEnable(bool _bCullingEnable)
	{
		mCullingAgent.reset();

		mbCullEnable = _bCullingEnable;
		if (mbCullEnable)
		{
			switch (mProjType)
			{
			case ehw::eProjectionType::Perspective:
				mCullingAgent = std::make_unique<CullingAgent_Perspective>();
				break;
			case ehw::eProjectionType::Orthographic:
				mCullingAgent = std::make_unique<CullingAgent_Orthographic>();
				break;
			default:
				ASSERT(nullptr, "Projection 타입이 설정되지 않았습니다.");
				break;
			}
		}
	}

	void Com_Camera::CreateProjectionMatrix()
	{
		uint2 resolution = GPUManager::GetResolution();
		CreateProjectionMatrix(resolution.x, resolution.y);
	}

	void Com_Camera::CreateProjectionMatrix(uint ResolutionX, uint ResolutionY)
	{
		mCullingAgent.reset();

		float width = (float)ResolutionX * m_scale;
		float height = (float)ResolutionY * m_scale;

		mAspectRatio = width / height;

		switch (mProjType)
		{
		case eProjectionType::Perspective:
			mProjection = MATRIX::CreatePerspectiveFieldOfViewLH
			(
				DirectX::XM_2PI / 6.0f
				, mAspectRatio
				, mNear
				, mFar
			);
			

			break;
		case eProjectionType::Orthographic:
			mProjection = MATRIX::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, mNear, mFar);

			break;

		default:
			ASSERT(false, "Projection Type이 설정되지 않았습니다.");
			break;
		}

		//Projection Mode가 변하면 Culling Agent도 교체 필요 -> 한 번 호출해줘야 한다
		SetCullEnable(mbCullEnable);
	}

	void Com_Camera::SetScale(float _scale)
	{
		if (_scale < 0.f)
			return;

		m_scale = _scale;
		CreateProjectionMatrix();
	}

	void Com_Camera::RegisterCameraInRenderer()
	{
		//eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		RenderManager::RegisterCamera(shared_from_this_T<Com_Camera>());
	}

	void Com_Camera::TurnLayerMask(uint32 _layer, bool _enable)
	{
		mLayerMasks.set((uint32)_layer, _enable);
	}


	//bool Com_Camera::FindRendererRecursive(GameObject* _pObj)
	//{
	//	bool bRet = false;
	//	if (_pObj)
	//	{
	//		if (_pObj->GetComponent(eComponentCategory::Renderer))
	//		{
	//			bRet = true;
	//		}
	//		else
	//		{
	//			//렌더러가 하나라도 있을 시 true 반환. 없을 시 false
	//			const auto& childs = _pObj->GetChilds();
	//			for (size_t i = 0; i < childs.size(); ++i)
	//			{
	//				bRet = FindRendererRecursive(childs[i]);
	//				if (bRet)
	//					break;
	//			}
	//		}
	//	}

	//	return bRet;
	//}

	void Com_Camera::SortGameObjects()
	{
		m_defferedOpaqueGameObjects.clear();
		m_forwardOpaqueGameObjects.clear();
		m_cutoutGameObjects.clear();
		m_transparentGameObject.clear();
		m_postProcessGameObjects.clear();

		iScene* scene = SceneManager::GetActiveScene();
		if (nullptr == scene)
		{
			return;
		}

		const std::vector<std::shared_ptr<GameObject>>& gameObjects = scene->GetGameObjects();

		for (size_t i = 0; i < gameObjects.size(); ++i)
		{
			if (true == mLayerMasks[gameObjects[i]->GetLayer()])
			{
				PushGameObjectToRenderingModes(gameObjects[i]);
			}
		}
	}

	void Com_Camera::RenderDeffered()
	{
		for (size_t i = 0; i < m_defferedOpaqueGameObjects.size(); ++i)
		{
			if (m_defferedOpaqueGameObjects[i])
			{
				m_defferedOpaqueGameObjects[i]->Render();
			}
		}
	}

	void Com_Camera::RenderForwardOpaque()
	{
		for (size_t i = 0; i < m_forwardOpaqueGameObjects.size(); ++i)
		{
			if (m_forwardOpaqueGameObjects[i])
			{
				m_forwardOpaqueGameObjects[i]->Render();
			}
		}
	}

	void Com_Camera::RenderCutout()
	{
		for (size_t i = 0; i < m_cutoutGameObjects.size(); ++i)
		{
			if (m_cutoutGameObjects[i])
			{
				m_cutoutGameObjects[i]->Render();
			}
		}
	}

	void Com_Camera::RenderTransparent()
	{
		for (size_t i = 0; i < m_transparentGameObject.size(); ++i)
		{
			if (m_transparentGameObject[i])
			{
				m_transparentGameObject[i]->Render();
			}
		}
	}

	void Com_Camera::RenderPostProcess()
	{
		for (size_t i = 0; i < m_postProcessGameObjects.size(); ++i)
		{
			if (m_postProcessGameObjects[i])
			{
				RenderManager::CopyRenderTarget();
				m_postProcessGameObjects[i]->Render();
			}
		}
	}

	void Com_Camera::PushGameObjectToRenderingModes(const std::shared_ptr<GameObject>& _gameObj)
	{
		if (nullptr == _gameObj || false == _gameObj->IsActive())
			return;

		const auto& renderer = _gameObj->GetComponent<iRenderer>();

		if (nullptr == renderer)
			return;
		
		//카메라 컬링 + 렌더러 컬링모드 활성화 시 컬링 진행
		else if (mCullingAgent && renderer->IsCullingEnabled())
		{
			
		}


		eRenderingMode mode = eRenderingMode::None;
		Material* mtrl = renderer->GetCurrentMaterial(0u);
		if (mtrl)
		{
			mode = mtrl->GetRenderingMode();
		}

		switch (mode)
		{
		case eRenderingMode::DefferdOpaque:
			[[fallthrough]];
		case eRenderingMode::DefferdMask:
			m_defferedOpaqueGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::Opaque:
			m_forwardOpaqueGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::CutOut:
			m_cutoutGameObjects.push_back(_gameObj);
			break;
		case eRenderingMode::Transparent:
			m_transparentGameObject.push_back(_gameObj);
			break;
		case eRenderingMode::PostProcess:
			m_postProcessGameObjects.push_back(_gameObj);
			break;
		default:
			break;
		}
	}


	Com_Camera::CullingAgent_Orthographic::CullingAgent_Orthographic()
	{
	}
	Com_Camera::CullingAgent_Orthographic::~CullingAgent_Orthographic()
	{
	}
	Com_Camera::CullingAgent_Perspective::CullingAgent_Perspective()
	{
	}
	Com_Camera::CullingAgent_Perspective::~CullingAgent_Perspective()
	{
	}
}
