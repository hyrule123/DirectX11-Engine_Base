#pragma once
#include <Engine/Util/AtExit.h>
#include <Engine/Common.h>

//!!! 주의사항: 생성자에서 다른 싱글턴 인스턴스를 호출하지 말 것 !!!
//! 안전을 위해 초기화는 init() 함수에서 진행할 것.

#define DECLARE_SINGLETON(T) \
public:\
static T& get_inst() { \
if (nullptr == m_instance) { m_instance = new T; m_instance->init(); } \
return *(m_instance); \
} \
static void destroy_inst() { SAFE_DELETE(m_instance); } \
private:\
static inline T* m_instance = nullptr;\
T(const T& _T) = delete;\
T(T&& _T) = delete


