#include "StaticMeshRenderer.h"

#include <Engine/Game/Component/Animator/Animator.h>

#include <Engine/Game/GameObject.h>

#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>

#include <Engine/Game/Component/Transform.h>

namespace core
{
	StaticMeshRenderer::StaticMeshRenderer() 
		: Renderer()
	{
	}

	StaticMeshRenderer::~StaticMeshRenderer()
	{
	}

	//void StaticMeshRenderer::render()
	//{
	//	//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
	//	if (false == is_render_ready())
	//		return;

	//	auto tr = get_owner()->get_component<Transform>();
	//	tr->bind_buffer_to_GPU_register();

	//	//Render
	//	auto mtrl = get_current_material();
	//	if (mtrl)
	//	{
	//		//메쉬 바인딩 - Mesh::Render 내부에서 진행하도록 변경
	//		//get_mesh()->BindBuffer(i);

	//		//재질 바인딩
	//		mtrl->bind_buffer_to_gpu_register();

	//		//메쉬 렌더링
	//		get_mesh()->render();
	//	}
	//}
}
