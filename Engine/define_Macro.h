#pragma once
#include <string>
#include <cassert>

#define CLASS_INFO(_class_, _super_) \
public: static inline constexpr const std::string_view concrete_class_name = #_class_;\
private: using Super = _super_

#ifdef _DEBUG

//release 모드에서도 작동
#define ASSERT(_expression, _c_str) \
	do \
	{ \
		if (!(_expression)) \
		{ _wassert(L ## #_expression##"\n\n"##_c_str , _CRT_WIDE(__FILE__), (unsigned)(__LINE__)); } \
	} \
	while (false)


#define ASSERT_DEBUG(_expression, _c_str) ASSERT(_expression, _c_str)

#define ERROR_MESSAGE_W(_c_str) \
	do { MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR); __debugbreak(); } while(false)
#define ERROR_MESSAGE_A(_c_str) \
	do { MessageBoxA(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR); __debugbreak(); } while(false)

#define DEBUG_MESSAGE_W(_c_str) \
	do { MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONINFORMATION); __debugbreak(); } while(false)
#define DEBUG_MESSAGE_A(_c_str) \
	do { MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONINFORMATION); __debugbreak(); } while(false)

#define DEBUG_LOG(_c_str) OutputDebugStringW(L ## _c_str)
#define DEBUG_LOG_A(_str) OutputDebugStringA(_str)
#define DEBUG_LOG_W(_wStr) OutputDebugStringW(_wStr)

#define DEBUG_BREAK ::__debugbreak()

#else //RELEASE
#define ASSERT(_expression, _c_str) \
	do \
	{ \
		if(!(_expression)) \
		{ \
		MessageBoxW(nullptr, L## #_expression##"\n\n"##_c_str, L"Assertion Failed!", MB_OK | MB_ICONERROR);\
		std::abort();\
		} \
	}\
	while(false)

#define ASSERT_DEBUG(_expression, _messageW) (void)0

#define ERROR_MESSAGE_W(_c_str) MessageBoxW(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR)
#define ERROR_MESSAGE_A(_c_str) MessageBoxA(nullptr, _c_str, nullptr, MB_OK | MB_ICONERROR)

#define DEBUG_MESSAGE_W(_c_str) 0
#define DEBUG_MESSAGE_A(_c_str) 0

#define DEBUG_LOG(_c_str) 0
#define DEBUG_LOG_A(_str) 0
#define DEBUG_LOG_W(_wStr) 0

#define DEBUG_BREAK 0

#endif _DEBUG

#define NOTIFICATION_W(_c_str) MessageBoxW(nullptr, _c_str, L"Notification", MB_OK)
#define NOTIFICATION_A(_c_str) MessageBoxA(nullptr, _c_str, "Notification", MB_OK)

#define ERROR_MESSAGE(_literalString) ERROR_MESSAGE_W(L##_literalString)
#define NOTIFICATION(_literalString) NOTIFICATION_W(L##_literalString)
#define DEBUG_MESSAGE(_literalString) DEBUG_MESSAGE_W(L##_literalString)

#define STR_KEY constexpr const std::string_view
#define STR_KEY_DECLARE(_key) STR_KEY _key = #_key

#define BIT_MASK(_MaskPos) 1 << _MaskPos

#define SAFE_DELETE(_ptr) if (_ptr) { delete _ptr; _ptr = nullptr; }
#define SAFE_DELETE_ARRAY(_ptr_arr) if (_ptr_arr) { delete[] _ptr_arr; _ptr_arr = nullptr; }

#define CONCAT_INNER(x, y) x ## y
#define CONCAT(x, y) CONCAT_INNER(x, y)
#define UNIQUE_VAR(x) CONCAT(x##_, __LINE__)
