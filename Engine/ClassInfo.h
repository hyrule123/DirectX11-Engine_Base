#pragma once
#include "Engine/Manager/InstanceManager.h"
#include <type_traits>

#define REGISTER_CLASS_INFO(_class_) \
private: static inline const bool unuse_1 = ClassInfo<_class_>::register_class(#_class_)

namespace ehw {
	template <typename T>
	class ClassInfo {
	public:
		static const std::string_view name() { return s_name; }

		static bool register_class(const std::string_view _name, bool _is_instance_able = false);

	private:
		static inline std::string_view s_name{};
		static inline bool s_is_instantiatable = false;
	};

	template <typename T>
	inline bool ClassInfo<T>::register_class(const std::string_view _name, bool _is_instance_able) {
		ASSERT(ClassInfo<T>::s_name.empty(), "Class Info를 중복 등록했습니다.");

		ClassInfo<T>::s_name = _name;
		ClassInfo<T>::s_is_instantiatable = _is_instance_able;

		if (_is_instance_able) {
			InstanceManager::GetInst().add_ctor<T>(_name);
		}

		return true;
	}
}

