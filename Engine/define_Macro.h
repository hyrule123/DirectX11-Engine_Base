#pragma once
#include <string>
#include <assert.h>

#define ASSERT(_expression, _message) if(!(_expression)) { \
		std::wstring assertMsg = \
		std::wstring( _CRT_WIDE_(#_expression) ) + \
		std::wstring(L"\n\nMessage: ") + \
		std::wstring( _CRT_WIDE_(_message) ); \
		_wassert(assertMsg.c_str(), _CRT_WIDE(__FILE__), (unsigned)(__LINE__)) ;\
	}



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

#define SAFE_DELETE(_ptr) if(_ptr) delete _ptr; _ptr = nullptr
#define SAFE_DELETE_ARRAY(_ptr) if(_ptr) delete[] _ptr; _ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }
