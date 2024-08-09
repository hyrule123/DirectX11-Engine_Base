#include "Engine/Scene/Component/Renderer/Com_Renderer_Mesh.h"

#include "Engine/Scene/Component/Animator/Animator.h"

#include "Engine/Scene/GameObject.h"

#include "Engine/Resource/Mesh.h"
#include "Engine/Resource/Material.h"


namespace ehw
{
	void Com_Renderer_Mesh::Render()
	{
		//메쉬 또는 재질이 없을 경우 렌더링 불가능이므로 return;
		if (false == IsRenderReady())
			return;

		auto tr = gameObject()->GetComponent<Com_Transform>();
		tr->BindData();

		//Render
		UINT iSubsetCount = GetMesh()->GetSubsetCount();
		for (UINT i = 0; i < iSubsetCount; ++i)
		{
			Material* mtrl = GetCurrentMaterial(i);
			if (mtrl)
			{
				//메쉬 바인딩 - Mesh::Render 내부에서 진행하도록 변경
				//GetMesh()->BindBuffer(i);

				//재질 바인딩
				mtrl->BindData();

				//메쉬 렌더링
				GetMesh()->Render(i);
			}
		}
	}
}
