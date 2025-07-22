#include "SkeletalMeshRenderer.h"

#include <Engine/Game/GameObject.h>

#include <Engine/Game/Component/Animator/Animator3D.h>
#include <Engine/Game/Component/Transform.h>

#include <Engine/Resource/Model3D/Skeleton.h>
#include <Engine/Resource/Mesh/Mesh.h>
#include <Engine/Resource/Material/Material.h>


namespace core
{
	SkeletalMeshRenderer::SkeletalMeshRenderer()
		: StaticMeshRenderer()
	{
	}

	SkeletalMeshRenderer::~SkeletalMeshRenderer()
	{
	}

	//void SkeletalMeshRenderer::render()
	//{
		//애니메이터가 없거나
		//3D 애니메이터가 아니거나
		//재생 중이 아니거나
		//->일반 Mesh 타입으로 렌더링
	//	auto animator = get_owner()->get_component<Animator3D>();
	//	if (
	//		nullptr == animator ||
	//		false == animator->is_playing()
	//		)
	//	{
	//		StaticMeshRenderer::render();
	//		return;
	//	}

	//	if (false == is_render_ready())
	//	{
	//		return;
	//	}
	//		

	//	auto tr = get_owner()->get_component<Transform>();
	//	tr->clear_buffer_data();
	//	tr->add_to_buffer();
	//	tr->bind_buffer_to_GPU_register();

	//	animator->bind_buffer_to_GPU_register();
	//	//Render
	//	UINT iSubsetCount = get_mesh()->get_subset_count();
	//	for (UINT i = 0; i < iSubsetCount; ++i)
	//	{
	//		Material* mtrl = get_current_material(i);
	//		if (mtrl)
	//		{
	//			//메쉬 바인딩 - Render 내부에서 실행되도록 변경
	//			//get_mesh()->BindBuffer(i);

	//			//재질에 애니메이션 정보 넣어주고 바인딩
	//			mtrl->SetAnim3D(true);
	//			mtrl->SetBoneCount(get_mesh()->get_skeleton()->get_bone_count());
	//			mtrl->bind_buffer_to_gpu_register();

	//			// 사용할 메쉬 업데이트 및 렌더링
	//			get_mesh()->render(i);

	//			//mtrl->unbind_buffer();
	//			mtrl->SetBoneCount(0);
	//			mtrl->SetAnim3D(false);
	//		}
	//	}

	//	animator->unbind_buffer_from_GPU_register();
	//}
	//}
}

