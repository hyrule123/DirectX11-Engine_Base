#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Game/Component/Animator/Animator.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Resource/Mesh/Mesh.h"
#include "Engine/Resource/Material/Material.h"

#include "Engine/Game/Component/Transform.h"

namespace core
{
	Com_Renderer_Mesh::Com_Renderer_Mesh() 
		: Renderer(Com_Renderer_Mesh::s_static_type_name)
	{
	}
	Com_Renderer_Mesh::Com_Renderer_Mesh(const std::string_view key)
		: Renderer(key)
	{
	}

	Com_Renderer_Mesh::~Com_Renderer_Mesh()
	{
	}

	//void Com_Renderer_Mesh::render()
	//{
	//	//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
	//	if (false == IsRenderReady())
	//		return;

	//	auto tr = get_owner()->get_component<Transform>();
	//	tr->bind_buffer_to_GPU_register();

	//	//Render
	//	auto mtrl = GetCurrentMaterial();
	//	if (mtrl)
	//	{
	//		//메쉬 바인딩 - Mesh::Render 내부에서 진행하도록 변경
	//		//GetMesh()->BindBuffer(i);

	//		//재질 바인딩
	//		mtrl->bind_buffer_to_gpu_register();

	//		//메쉬 렌더링
	//		GetMesh()->render();
	//	}
	//}
}
