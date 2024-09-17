#pragma once
#include "Engine/Util/StaticSingleton.h"

#include <FW1FontWrapper/FW1FontWrapper.h>
#include <FW1FontWrapper/FW1CompileSettings.h>
namespace ehw
{
	class FontWrapper : public StaticSingleton<FontWrapper>
	{
		friend class StaticSingleton<FontWrapper>;
		friend class GameEngine;
	public:
		void DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint32 _rgb);

	private:
		FontWrapper();
		~FontWrapper();
		bool init();
		void release();

	private:
		IFW1Factory* m_FW1Factory;
		IFW1FontWrapper* m_fontWrapper;
	};
}
