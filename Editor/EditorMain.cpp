#include "Editor/PCH_Editor.h"
#include "Editor/EditorMain.h"

#include "Engine/GameEngine.h"

#include "Editor/EditorManager.h"

namespace core::editor
{
	EditorMain::EditorMain(const tDesc_EngineMain& _desc)
		: m_engineMain(_desc)
	{
		GameEngine::get_inst().set_editor_func(EditorManager::GetEditorRunFunction());
	}
	EditorMain::~EditorMain()
	{
	}
	BOOL EditorMain::run()
	{
		return m_engineMain.run();
	}
}
