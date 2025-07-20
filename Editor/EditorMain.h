#pragma once
#include <Engine/EngineMain.h>

namespace core::editor
{
	class EditorMain
	{
	public:
		EditorMain(const tDesc_EngineMain& _desc);
		~EditorMain();

		BOOL run();

		EngineMain m_engine_main;
	};
}


