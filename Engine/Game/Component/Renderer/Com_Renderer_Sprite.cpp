#include "Engine/Game/Component/Renderer/Com_Renderer_Sprite.h"
#include "Engine/Game/Component/Com_Transform.h"
#include "Engine/Game/Component/Animator/Com_Animator2D.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"

#include "Engine/Game/GameObject.h"



namespace ehw
{
	Com_Renderer_Sprite::Com_Renderer_Sprite()
		: Com_Renderer_Mesh(ClassInfo<Com_Renderer_Sprite>::name())
		, mAnimator(nullptr)
	{
	}

	Com_Renderer_Sprite::~Com_Renderer_Sprite()
	{
	}

	void Com_Renderer_Sprite::Init()
	{
		//Renderer가 렌더링할 때 필요한 것들
		//1. 메쉬
		//2. 재질
		//	재질 1. 쉐이더
		//	재질 2. 텍스처

		//기본 Rect Mesh로 메쉬를 설정
		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::GetInst().Find(strKey::defaultRes::mesh::RectMesh);
		SetMesh(mesh);

		//Sprite Material을 받아온다
		std::shared_ptr<Material> material = ResourceManager<Material>::GetInst().Find(strKey::defaultRes::material::SpriteMaterial);
		SetMaterial(material, 0);
	}

	void Com_Renderer_Sprite::Awake()
	{
		mAnimator = gameObject()->GetComponent<Com_Animator2D>();
	}

	void Com_Renderer_Sprite::Render()
	{
		if (false == IsRenderReady() || nullptr == mAnimator)
			return;

		//재질 바인딩
		mAnimator->BindData();

		//메쉬 바인딩 - Render 내부에서 실행되도록 변경
		//GetMesh()->BindBuffer();

		//재질 바인딩
		GetCurrentMaterial(0)->BindData();

		//렌더링 하고
		GetMesh()->Render();

		//재질 언바인딩
		//GetCurrentMaterial(0)->UnbindData();

		//애니메이터 언바인드
		//mAnimator->UnBindData();
	}
}
