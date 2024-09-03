#pragma once
#include "Engine/Manager/InstanceManager.h"
#include <type_traits>

#define REGISTER_CLASS(_class_, _is_instance_able) \
private: static inline const bool unuse = ClassInfo<_class_>::register_class<_is_instance_able>(#_class_)

#define REGISTER_CLASS_NAME_ONLY(_class_) REGISTER_CLASS(_class_, false)
#define REGISTER_CLASS_INSTANCE_ABLE(_class_) REGISTER_CLASS(_class_, true)

namespace ehw {
	template <typename T>
	class ClassInfo {
	public:
		static const std::string_view name() { return s_name; }

		template <bool Is_Instance_Able>
		static bool register_class(const std::string_view _name);

	private:
		static inline std::string_view s_name{};
		static inline bool s_is_instantiatable = false;
	};

	template <typename T>
	template <bool Is_Instance_Able>
	inline bool ClassInfo<T>::register_class(const std::string_view _name) {
		ClassInfo<T>::s_name = _name;
		ClassInfo<T>::s_is_instantiatable = Is_Instance_Able;

		if constexpr (Is_Instance_Able) {
			InstanceManager::GetInst().add_ctor<T>(_name);
		}

		return true;
	}
}

