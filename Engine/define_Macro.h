#pragma once
#include <string>

//매크로에서 호출하는 함수. 직접 호출하지 말것
inline void AssertW(const wchar_t* _expression, const wchar_t* _message, const wchar_t* _fileName, unsigned int _line)
{
	std::wstring assertMsg = std::wstring(_expression) + std::wstring(L"\n\nMessage: ") + std::wstring(_message);
	_wassert(assertMsg.c_str(), _fileName, _line);
}

#define ASSERT(_expression, _message) (void) (					\
	(!!(_expression)) || \
	(AssertW(_CRT_WIDE(#_expression), _CRT_WIDE(_message), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)), 0) \
)

#ifdef _DEBUG
#define ASSERT_DEBUG(_expression, _message) ASSERT(_expression, _message)
#else
#define ASSERT_DEBUG(_expression, _message)
#endif _DEBUG

#define STRKEY constexpr const char*
#define STRKEY_DECLARE(_AStr) STRKEY _AStr = #_AStr

#ifdef _DEBUG
#define DEBUG_BREAK DebugBreak()
#else
#define DEBUG_BREAK
#endif

#define ERROR_MESSAGE_A(_aStrMessage) MessageBoxA(nullptr, _aStrMessage, NULL, MB_OK); DEBUG_BREAK
#define ERROR_MESSAGE_W(_wStrMessage) MessageBoxW(nullptr, _wStrMessage, NULL, MB_OK); DEBUG_BREAK

#define NOTIFICATION_W(_wStrMessage) MessageBoxW(nullptr, _wStrMessage, L"알림", MB_OK)

#define BIT_MASK(_MaskPos) 1 << _MaskPos

#define SAFE_DELETE(_Ptr) if(_Ptr) delete _Ptr; _Ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }
