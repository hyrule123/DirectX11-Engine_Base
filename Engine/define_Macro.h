#pragma once
#include <string>
#include <assert.h>

#ifdef _DEBUG

#define ASSERT(_expression, _message) if (!(_expression)) { assert(!_message); }
//#define ASSERT(expr, msg) assert(( (void)(msg), (expr) ))
#define ASSERT_DEBUG(_expression, _message) ASSERT(_expression, _message)

#else
#define ASSERT(_expression, _message) if(!(_expression)) { \
		MessageBoxW(nullptr, L##_message, L"Assertion Failed!", MB_OK | MB_ICONERROR);\
		__debugbreak();\
	}
#define ASSERT(_expression, _messageW) 0
#endif _DEBUG	

#define ERROR_MESSAGE_W(_message) MessageBoxW(nullptr, _message, nullptr, MB_OK | MB_ICONERROR)
#define ERROR_MESSAGE_A(_message) MessageBoxA(nullptr, _message, nullptr, MB_OK | MB_ICONERROR)

#define NOTIFICATION_W(_message) MessageBoxW(nullptr, _message, L"Notification", MB_OK)
#define NOTIFICATION_A(_message) MessageBoxA(nullptr, _message, "Notification", MB_OK)


#define STRKEY constexpr const char*
#define STRKEY_DECLARE(_AStr) STRKEY _AStr = #_AStr


#define BIT_MASK(_MaskPos) 1 << _MaskPos

#define SAFE_DELETE(_ptr) if(_ptr) delete _ptr; _ptr = nullptr
#define SAFE_DELETE_ARRAY(_ptr) if(_ptr) delete[] _ptr; _ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }
