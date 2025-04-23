#include "Editor/PCH_Editor.h"
#include "Editor/EditorMain.h"

#include "Engine/GameEngine.h"

#include "Editor/EditorManager.h"

namespace core::editor
{
	EditorMain::EditorMain(const tDesc_EngineMain& _desc)
		: m_engineMain(_desc)
	{
	}
	EditorMain::~EditorMain()
	{
	}
	BOOL EditorMain::Run()
	{
		return m_engineMain.Run();
	}
}
