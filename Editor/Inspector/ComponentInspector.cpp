#include <Editor/Inspector/ComponentInspector.h>



namespace core::editor
{
	ComponentInspector::ComponentInspector(eComponentOrder _type)
		: EditorUIWindow(::core::name::eComponentCategory_String[(int)_type])
		, m_component_order(_type)
		, m_target_gameobject()
	{
	}

	ComponentInspector::~ComponentInspector()
	{

	}

}
