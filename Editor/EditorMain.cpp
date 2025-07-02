#include <Editor/PCH_Editor.h>
#include "EditorMain.h"

#include "Engine/GameEngine.h"

#include <Editor/EditorManager.h>

namespace core::editor
{
	EditorMain::EditorMain(const tDesc_EngineMain& _desc)
		: m_engineMain(_desc)
	{
	}
	EditorMain::~EditorMain()
	{
	}
	BOOL EditorMain::run()
	{
		GameEngine::get_inst().set_editor_func(EditorManager::get_inst().get_editor_run_func());
		return m_engineMain.run();
	}
}
