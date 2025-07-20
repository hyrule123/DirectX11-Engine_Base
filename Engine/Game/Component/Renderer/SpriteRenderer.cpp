#include "SpriteRenderer.h"
#include <Engine/Game/Component/Transform.h>
#include <Engine/Game/Component/Animator/Animator2D.h>

#include <Engine/Manager/ResourceManager.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

#include <Engine/Game/GameObject.h>



namespace core
{
	SpriteRenderer::SpriteRenderer()
		: StaticMeshRenderer(SpriteRenderer::s_static_type_name)
		, m_animator()
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}

	void SpriteRenderer::init()
	{
		//Renderer가 렌더링할 때 필요한 것들
		//1. 메쉬
		//2. 재질
		//	재질 1. 쉐이더
		//	재질 2. 텍스처

		//기본 Rect Mesh로 메쉬를 설정
		s_ptr<Mesh> mesh = ResourceManager<Mesh>::get_inst().find(name::defaultRes::mesh::RectMesh);
		set_mesh(mesh);

		//Sprite Material을 받아온다
		s_ptr<Material> material = ResourceManager<Material>::get_inst().find(name::defaultRes::material::SpriteMaterial);
		set_material(material);
	}

	void SpriteRenderer::awake()
	{
		m_animator = get_owner()->get_component<Animator2D>();
	}

	//void SpriteRenderer::render()
	//{
	//	if (false == is_render_ready() || nullptr == m_animator)
	//		return;

	//	//재질 바인딩
	//	m_animator->bind_buffer_to_GPU_register();

	//	//메쉬 바인딩 - Render 내부에서 실행되도록 변경
	//	//get_mesh()->BindBuffer();

	//	//재질 바인딩
	//	get_current_material()->bind_buffer_to_gpu_register();

	//	//렌더링 하고
	//	get_mesh()->render();

	//	//재질 언바인딩
	//	//get_current_material(0)->unbind_buffer();

	//	//애니메이터 언바인드
	//	//m_animator->unbind_buffer_from_GPU_register();
	//}
}
