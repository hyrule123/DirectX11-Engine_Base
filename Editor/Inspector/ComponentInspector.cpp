#include <Editor/Inspector/ComponentInspector.h>



namespace core::editor
{
	ComponentInspector::ComponentInspector(eComponentOrder _type)
		: EditorUIWindow()
		, m_component_order(_type)
		, m_target_gameobject()
	{
	}

	void ComponentInspector::init()
	{
		Super::init();

		set_unique_name(::core::name::eComponentCategory_String[(int)m_component_order]);
	}

	ComponentInspector::~ComponentInspector()
	{

	}

}
