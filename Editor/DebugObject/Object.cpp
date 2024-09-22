
#include "Editor/DebugObject/Object.h"

namespace ehw::editor
{
	EditorObject::EditorObject()
		: GameObject()
	{
	}
	EditorObject::~EditorObject()
	{
		
	}
	void EditorObject::Awake()
	{
		GameObject::Awake();
	}
	void EditorObject::update()
	{
		GameObject::update();
	}
	void EditorObject::final_update()
	{
		GameObject::final_update();
	}
	void EditorObject::render()
	{
		GameObject::render();
	}
}
