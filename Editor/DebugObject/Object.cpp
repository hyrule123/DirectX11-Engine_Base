
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
	void EditorObject::Update()
	{
		GameObject::Update();
	}
	void EditorObject::FinalUpdate()
	{
		GameObject::FinalUpdate();
	}
	void EditorObject::Render()
	{
		GameObject::Render();
	}
}
