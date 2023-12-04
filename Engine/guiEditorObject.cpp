#include "PCH_Engine.h"

#include "guiEditorObject.h"

namespace gui
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
	void EditorObject::Update()
	{
		GameObject::Update();
	}
	void EditorObject::InternalUpdate()
	{
		GameObject::InternalUpdate();
	}
	void EditorObject::Render()
	{
		GameObject::Render();
	}
}
