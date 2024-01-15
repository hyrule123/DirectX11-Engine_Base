#pragma once
#include <string>
#include <cassert>



#ifdef _DEBUG

//Release 모드에서도 작동
#define ASSERT(_expression, _c_str) \
	if (!(_expression)) \
	{ _wassert(L ## #_expression##"\n\n"##_c_str , _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); }


#define ASSERT_DEBUG(_expression, _c_str) ASSERT(_expression, _c_str)

#define ERROR_MESSAGE_W(_c_str) MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR); __debugbreak()
#define ERROR_MESSAGE_A(_c_str) MessageBoxA(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR); __debugbreak()

#define DEBUG_MESSAGE_W(_c_str) MessageBoxW(nullptr, L ## _c_str, nullptr, MB_OK | MB_ICONINFORMATION); __debugbreak()
#define DEBUG_MESSAGE_A(_c_str) MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONINFORMATION); __debugbreak()

#define DEBUG_LOG(_c_str) OutputDebugStringW(L ## _c_str)

#else
#define ASSERT(_expression, _c_str) if(!(_expression)) { \
		MessageBoxW(nullptr, L## #_expression##"\n\n"##_c_str, L"Assertion Failed!", MB_OK | MB_ICONERROR);\
		std::abort();\
	}

#define ASSERT_DEBUG(_expression, _messageW) 0

#define ERROR_MESSAGE_W(_c_str) MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR)
#define ERROR_MESSAGE_A(_c_str) MessageBoxA(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR)

#define DEBUG_MESSAGE_W(_c_str) 0
#define DEBUG_MESSAGE_A(_c_str) 0

#endif _DEBUG


#define NOTIFICATION_W(_c_str) MessageBoxW(nullptr, _c_str, L"Notification", MB_OK)
#define NOTIFICATION_A(_c_str) MessageBoxA(nullptr, _c_str, "Notification", MB_OK)


#define ERROR_MESSAGE(_literalString) ERROR_MESSAGE_W(L##_literalString)
#define NOTIFICATION(_literalString) NOTIFICATION_W(L##_literalString)
#define DEBUG_MESSAGE(_literalString) DEBUG_MESSAGE_W(L##_literalString)


#define STRKEY constexpr const char*
#define STRKEY_DECLARE(_AStr) STRKEY _AStr = #_AStr


#define BIT_MASK(_MaskPos) 1 << _MaskPos

#define SAFE_DELETE(_ptr) if(_ptr) delete _ptr; _ptr = nullptr
#define SAFE_DELETE_ARRAY(_ptr) if(_ptr) delete[] _ptr; _ptr = nullptr

#define CLONE(_type) virtual _type* Clone() override { return new _type(*this); }
#define CLONE_DISABLE(_type) virtual _type* Clone() override { ERROR_MESSAGE_W(L"Clone할 수 없거나 Clone함수를 재정의하지 않은 클래스입니다."); return nullptr; }
