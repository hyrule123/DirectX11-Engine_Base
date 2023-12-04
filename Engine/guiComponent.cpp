#include "PCH_Engine.h"

#include "guiComponent.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

namespace gui
{
	guiComponent::guiComponent(ehw::eComponentCategory _type)
		: guiChild(ehw::strKey::eComponentType_String[(int)_type])
		, m_ComCategory(_type)
		, mTarget(nullptr)
	{
	}

	guiComponent::~guiComponent()
	{

	}

}
