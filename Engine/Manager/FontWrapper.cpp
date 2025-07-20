
#include "FontWrapper.h"



#include <Engine/Util/AtExit.h>


#include <Engine/Manager/RenderManager.h>


namespace core
{
	FontWrapper::FontWrapper()
		: m_FW1_factory(nullptr)
		, m_font_wrapper(nullptr)
	{}
	void FontWrapper::init()
	{
		HRESULT result = FW1CreateFactory(FW1_VERSION, &m_FW1_factory);
		ASSERT(SUCCEEDED(result) ,"FW1CreateFactory 실패");

		auto device = RenderManager::get_inst().get_device();

		result = m_FW1_factory->CreateFontWrapper(device.Get(), L"Arial", &m_font_wrapper);

		ASSERT(SUCCEEDED(result), "CreateFontWrapper 실패");

		AtExit::add_func(FontWrapper::destroy_inst);
	}

	FontWrapper::~FontWrapper()
	{
		m_FW1_factory->Release();
		m_FW1_factory = nullptr;

		m_font_wrapper->Release();
		m_font_wrapper = nullptr;
	}

	void FontWrapper::draw_font(const wchar_t* _string, float _x, float _y, float _size, uint32 _rgb)
	{
		auto context = RenderManager::get_inst().get_context();
		m_font_wrapper->DrawString(context.Get(),
								 _string, // String
								 _size,// Font size
								 _x,// X position
								 _y,// Y position
								 _rgb,// Text color, 0xAaBbGgRr
								 0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
								);
	}
}
