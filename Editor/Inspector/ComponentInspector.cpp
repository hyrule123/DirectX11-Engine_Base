#include "Editor/Inspector/ComponentInspector.h"



namespace core::editor
{
	ComponentInspector::ComponentInspector(eComponentOrder _type)
		: EditorWindow(::core::name::eComponentCategory_String[(int)_type])
		, m_ComCategory(_type)
		, mTarget()
	{
	}

	ComponentInspector::~ComponentInspector()
	{

	}

}
