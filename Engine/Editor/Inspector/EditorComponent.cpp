
#include "EditorComponent.h"

#include "../../Editor/imgui/imgui.h"
#include "../../Editor/imgui/imgui_impl_win32.h"
#include "../../Editor/imgui/imgui_impl_dx11.h"

namespace editor
{
	EditorComponent::EditorComponent(ehw::eComponentCategory _type)
		: EditorChild(ehw::strKey::eComponentCategory_String[(int)_type])
		, m_ComCategory(_type)
		, mTarget()
	{
	}

	EditorComponent::~EditorComponent()
	{

	}

}
