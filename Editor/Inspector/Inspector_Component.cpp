#include "Editor/Inspector/Inspector_Component.h"



namespace core::editor
{
	Inspector_Component::Inspector_Component(eComponentOrder _type)
		: EditorChild(::core::name::eComponentCategory_String[(int)_type])
		, m_ComCategory(_type)
		, mTarget()
	{
	}

	Inspector_Component::~Inspector_Component()
	{

	}

}
