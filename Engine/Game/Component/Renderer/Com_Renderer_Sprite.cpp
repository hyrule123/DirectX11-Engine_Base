#include "Engine/Game/Component/Renderer/Com_Renderer_Sprite.h"
#include "Engine/Game/Component/Transform.h"
#include "Engine/Game/Component/Animator/Com_Animator2D.h"

#include "Engine/Manager/ResourceManager.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/Game/GameObject.h"



namespace ehw
{
	Com_Renderer_Sprite::Com_Renderer_Sprite()
		: Com_Renderer_Mesh(Com_Renderer_Sprite::concrete_class_name)
		, mAnimator(nullptr)
	{
	}

	Com_Renderer_Sprite::~Com_Renderer_Sprite()
	{
	}

	void Com_Renderer_Sprite::init()
	{
		//Renderer가 렌더링할 때 필요한 것들
		//1. 메쉬
		//2. 재질
		//	재질 1. 쉐이더
		//	재질 2. 텍스처

		//기본 Rect Mesh로 메쉬를 설정
		std::shared_ptr<Mesh> mesh = ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::RectMesh);
		set_mesh(mesh);

		//Sprite Material을 받아온다
		std::shared_ptr<Material> material = ResourceManager<Material>::get_inst().find(name::defaultRes::material::SpriteMaterial);
		set_material(material);
	}

	void Com_Renderer_Sprite::Awake()
	{
		mAnimator = gameObject()->GetComponent<Com_Animator2D>();
	}

	//void Com_Renderer_Sprite::render()
	//{
	//	if (false == IsRenderReady() || nullptr == mAnimator)
	//		return;

	//	//재질 바인딩
	//	mAnimator->bind_buffer_to_GPU_register();

	//	//메쉬 바인딩 - Render 내부에서 실행되도록 변경
	//	//GetMesh()->BindBuffer();

	//	//재질 바인딩
	//	GetCurrentMaterial()->bind_buffer_to_gpu_register();

	//	//렌더링 하고
	//	GetMesh()->render();

	//	//재질 언바인딩
	//	//GetCurrentMaterial(0)->unbind_buffer();

	//	//애니메이터 언바인드
	//	//mAnimator->unbind_buffer_from_GPU_register();
	//}
}
