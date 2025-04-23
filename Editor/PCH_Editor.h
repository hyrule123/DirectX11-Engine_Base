#pragma once

#pragma comment(lib, "Engine.lib")
#include <Engine/PCH_Engine.h>

#ifdef ENGINE_MAIN
#undef ENGINE_MAIN
#include "Editor/EditorMain.h"
#define ENGINE_MAIN core::editor::EditorMain
#endif
