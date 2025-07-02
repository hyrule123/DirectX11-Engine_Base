#pragma once
#include <Engine/Util/Singleton.h>

#include <FW1FontWrapper/FW1FontWrapper.h>
#include <FW1FontWrapper/FW1CompileSettings.h>
namespace core
{
	class FontWrapper
	{
		friend class GameEngine;

		DECLARE_SINGLETON(FontWrapper);
	private:
		FontWrapper();
		~FontWrapper();

	public:
		void DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint32 _rgb);

	private:
		IFW1Factory* m_FW1Factory;
		IFW1FontWrapper* m_fontWrapper;
	};
}
