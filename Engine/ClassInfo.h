#pragma once
#include "Engine/Manager/InstanceManager.h"
#include <type_traits>

#define REGISTER_CLASS_INFO(_class_) \
private: static inline const bool unuse_1 = ClassInfo<_class_>::register_class(#_class_)
#define SET_INSTANCE_ABLE(_class_) \
private: static inline const bool unuse_2 = ClassInfo<_class_>::set_instantiable(true)

namespace ehw {
	template <typename T>
	class ClassInfo {
	public:
		static const std::string_view name() { return s_name; }

		static bool register_class(const std::string_view _name);
		static bool set_instantiable(bool _is_instantiable);

	private:
		static inline std::string_view s_name{};
		static inline bool s_is_instantiatable = false;
	};

	template <typename T>
	inline bool ClassInfo<T>::register_class(const std::string_view _name) {
		ASSERT(ClassInfo<T>::s_name.empty(), "Class Info를 중복 등록했습니다.");
		ClassInfo<T>::s_name = _name;
		return true;
	}

	template <typename T>
	bool ClassInfo<T>::set_instantiable(bool _is_instantiable) {
		ASSERT(false == ClassInfo<T>::s_name.empty(), "Class Info 이름이 등록되지 않았습니다.");
		ClassInfo<T>::s_is_instantiatable = _is_instantiable;
		if (_is_instantiable) {
			InstanceManager::GetInst().add_ctor<T>(ClassInfo<T>::s_name);
		}
		return true;
	}
}

