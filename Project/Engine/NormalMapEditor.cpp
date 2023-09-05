#include "PCH_Engine.h"
#include "NormalMapEditor.h"

namespace mh
{
	NormalMapEditor::NormalMapEditor()
		: ComputeShader(uint3{32, 32, 1})
		, mTarget(nullptr)
	{
	}

	NormalMapEditor::~NormalMapEditor()
	{
	}

	std::shared_ptr<Texture> NormalMapEditor::GetModifiedNormalMap(std::shared_ptr<Texture> _texture)
	{
		return std::shared_ptr<Texture>();
	}

	bool NormalMapEditor::BindData()
	{
		mTarget->BindDataUAV(0);

		uint3 dataCounts = { mTarget->GetWidth(), mTarget->GetHeight(), 1u };

		CalculateGroupCount(dataCounts);

		return true;
	}

	void NormalMapEditor::UnBindData()
	{
		mTarget->UnBindData();
	}
}
