#include "Editor/Inspector/Inspector_Component.h"



namespace ehw::editor
{
	Inspector_Component::Inspector_Component(eComponentCategory _type)
		: EditorChild(::ehw::name::eComponentCategory_String[(int)_type])
		, m_ComCategory(_type)
		, mTarget()
	{
	}

	Inspector_Component::~Inspector_Component()
	{

	}

}
