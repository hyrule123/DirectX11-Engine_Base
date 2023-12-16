#pragma once
#include <string>
#include <assert.h>

#ifdef _DEBUG

//Release 모드에서도 작동
#define ASSERT(_expression, _message) \
	if (!(_expression)) \
	{ _wassert(L ## #_expression##"\n\n"##_message , _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); }

//return 코드가 필요한 경우 사용(Release 모드에서도 작동)
//#define ASSERT_RETURN_IF_FAIL(_expression, _message, _returnVal) \
//	if (!(_expression)) \
//	{ \
//		_wassert(L ## #_expression##"\n"##_message, _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); \
//		return _returnVal; \
//	}

//#define ASSERT(_expression, _message) if (!(_expression)) { assert(!_message); }
//#define ASSERT(expr, msg) assert(( (void)(msg), (expr) ))
#define ASSERT_DEBUG(_expression, _message) ASSERT(_expression, _message)

#define ERROR_MESSAGE_W(_message) MessageBoxW(nullptr, _message, nullptr, MB_OK | MB_ICONERROR); __debugbreak()
#define ERROR_MESSAGE_A(_message) MessageBoxA(nullptr, _message, nullptr, MB_OK | MB_ICONERROR); __debugbreak()

#else
#define ASSERT(_expression, _message) if(!(_expression)) { \
		MessageBoxW(nullptr, L## #_expression##"\n\n"##_message, L"Assertion Failed!", MB_OK | MB_ICONERROR);\
		std::abort();\
	}
//#define ASSERT_RETURN_IF_FAIL(_expression, _message, _returnVal) if(!(_expression)) { \
//		MessageBoxW(nullptr, L## #_expression##"\n"##_message, L"Assertion Failed!", MB_OK | MB_ICONERROR);\
//		std::abort();\
//		return _returnVal;\
//	}
#define ASSERT_DEBUG(_expression, _messageW) 0

#define ERROR_MESSAGE_W(_message) MessageBoxW(nullptr, _message, nullptr, MB_OK | MB_ICONERROR)
#define ERROR_MESSAGE_A(_message) MessageBoxA(nullptr, _message, nullptr, MB_OK | MB_ICONERROR)

#endif _DEBUG


#define NOTIFICATION_W(_message) MessageBoxW(nullptr, _message, L"Notification", MB_OK)
#define NOTIFICATION_A(_message) MessageBoxA(nullptr, _message, "Notification", MB_OK)


#define STRKEY constexpr const char*
#define STRKEY_DECLARE(_AStr) STRKEY _AStr = #_AStr


#define BIT_MASK(_MaskPos) 1 << _MaskPos

#define SAFE_DELETE(_ptr) if(_ptr) delete _ptr; _ptr = nullptr
#define SAFE_DELETE_ARRAY(_ptr) if(_ptr) delete[] _ptr; _ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }
