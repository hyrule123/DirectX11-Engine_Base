
#include "FontWrapper.h"



#include "Engine/Util/AtExit.h"


#include "GPUManager.h"


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment(lib, "FW1FontWrapper/x64/Debug/FW1FontWrapperL.lib")
#else _DEBUG
#pragma comment(lib, "FW1FontWrapper/x64/Release/FW1FontWrapper.lib")
#endif _DEBUG


#else _WIN64
#error "x64만 지원합니다"
#endif _WIN64





namespace ehw
{
	FontWrapper::FontWrapper()
		: m_FW1Factory{ nullptr }
		, m_fontWrapper{ nullptr }
	{
	}

	FontWrapper::~FontWrapper()
	{
	}

	bool FontWrapper::Init()
	{
		AtExit::AddFunc(std::bind(&FontWrapper::Release, this));

		if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
			return false;

		auto pDevice = GPUManager::GetInst().Device();
		if (FAILED(m_FW1Factory->CreateFontWrapper(pDevice, L"Arial", &m_fontWrapper)))
			return false;

		return true;
	}

	void FontWrapper::DrawFont(const wchar_t* _string, float _x, float _y, float _size, uint32 _rgb)
	{
		auto context = GPUManager::GetInst().Context();
		m_fontWrapper->DrawString(context,
								 _string, // String
								 _size,// Font size
								 _x,// X position
								 _y,// Y position
								 _rgb,// Text color, 0xAaBbGgRr
								 0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
								);
	}

	void FontWrapper::Release()
	{
		m_FW1Factory->Release();
		m_FW1Factory = nullptr;

		m_fontWrapper->Release();
		m_fontWrapper = nullptr;
	}
}
