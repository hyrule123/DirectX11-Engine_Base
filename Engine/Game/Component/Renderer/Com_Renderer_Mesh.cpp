#include "Engine/Game/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Game/Component/Animator/iAnimator.h"

#include "Engine/Game/GameObject.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"


namespace ehw
{
	void Com_Renderer_Mesh::Render()
	{
		//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
		if (false == IsRenderReady())
			return;

		auto tr = GetOwner()->GetComponent<Com_Transform>();
		tr->BindData();

		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				//메쉬 바인딩
				GetMesh()->BindBuffer(i);

				//재질 바인딩
				GetCurrentMaterial(i)->BindData();

				//메쉬 렌더링
				GetMesh()->Render(i);

				//데이터 연결 해제
				GetCurrentMaterial(i)->UnBindData();
			}
		}
	}
}
