
#include "FontWrapper.h"



#include <Engine/Util/AtExit.h>


#include <Engine/Manager/RenderManager.h>


namespace core
{
	FontWrapper::FontWrapper()
		: m_FW1Factory{ nullptr }
		, m_fontWrapper{ nullptr }
	{
		AtExit::add_func(FontWrapper::destroy_inst);

		ASSERT(
			SUCCEEDED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory))
			,"FW1CreateFactory 실패");

		auto pDevice = RenderManager::get_inst().Device();
		ASSERT(
			SUCCEEDED(m_FW1Factory->CreateFontWrapper(pDevice, L"Arial", &m_fontWrapper))
			, "CreateFontWrapper 실패");
	}

	FontWrapper::~FontWrapper()
	{
		m_FW1Factory->Release();
		m_FW1Factory = nullptr;

		m_fontWrapper->Release();
		m_fontWrapper = nullptr;
	}

	void FontWrapper::DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint32 _rgb)
	{
		auto context = RenderManager::get_inst().Context();
		m_fontWrapper->DrawString(context,
								 _string, // String
								 _size,// Font size
								 _x,// X position
								 _y,// Y position
								 _rgb,// Text color, 0xAaBbGgRr
								 0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
								);
	}
}
