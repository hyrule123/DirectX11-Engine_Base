#include "Com_Renderer_3DAnimMesh.h"


#include "../../../Game/GameObject.h"
#include "../../../Resource/Model3D/Skeleton.h"


#include "../Animator/Com_Animator3D.h"


namespace ehw
{
	Com_Renderer_3DAnimMesh::Com_Renderer_3DAnimMesh()
	{
	}

	Com_Renderer_3DAnimMesh::~Com_Renderer_3DAnimMesh()
	{
	}

	void Com_Renderer_3DAnimMesh::Render()
	{
		//애니메이터가 없거나
		//3D 애니메이터가 아니거나
		//재생 중이 아니거나
		//->일반 Mesh 타입으로 렌더링
		iAnimator* animator = GetOwner()->GetComponent<iAnimator>();
		if (
			nullptr == animator 
			|| eDimensionType::_3D != animator->GetDimensionType()
			|| false == animator->IsPlaying()
			)
		{
			Com_Renderer_Mesh::Render();
			return;
		}

		if (false == IsRenderReady())
			return;

		iTransform* tr = 
			static_cast<iTransform*>(GetOwner()->GetComponent(eComponentCategory::Transform));
		tr->BindData();

		animator->BindData();
		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				//메쉬 바인딩
				GetMesh()->BindBuffer(i);

				//재질에 애니메이션 정보 넣어주고 바인딩
				mtrl->SetAnim3D(true);
				mtrl->SetBoneCount(GetMesh()->GetSkeleton()->GetBoneCount());
				mtrl->BindData();

				// 사용할 메쉬 업데이트 및 렌더링
				GetMesh()->Render(i);

				mtrl->UnBindData();
				mtrl->SetBoneCount(0);
				mtrl->SetAnim3D(false);
			}
		}

		animator->UnBindData();
	}
}

