#pragma once
//#include "Engine/Manager/InstanceManager.h"
#include "Engine/Entity.h"
#include <type_traits>

#define INSTANCE_ABLE(_class_) ClassRegisterHelper<_class_, #_class_, true>::get_name()
#define INSTANCE_UNABLE(_class_) ClassRegisterHelper<_class_, #_class_, false>::get_name()

namespace ehw {
	template<unsigned N>
	struct FixedString
	{
		char buf[N + 1]{};

		constexpr FixedString(char const* s)
		{
			for (unsigned i = 0; i != N; ++i) buf[i] = s[i];
		}
		constexpr operator char const* () const { return buf; }

		// not mandatory anymore
		auto operator<=>(const FixedString&) const = default;
	};
	template<unsigned N> FixedString(char const (&)[N]) -> FixedString<N - 1>;

	template <typename T>
	class ClassInfo {
	public:
		inline static const std::string_view get_name() { return s_name; }
	protected:
		inline static std::string_view s_name;
	};

	//최초 선언 시 타입에 대한 문자열을 등록한다.
	template <typename T, FixedString S, bool Is_Instantiable>
	class ClassRegisterHelper
		: public ClassInfo<T>
	{
	private:
		static const int register_inst() {
			ClassInfo<T>::s_name = std::string_view(S);
			//if constexpr (Is_Instantiable) {
			//	InstanceManager::GetInst().add_ctor<T>(ClassInfo<T>::s_name);
			//}
			return 1;
		}
		//처음 1회 초기화를 위한 변수
		static inline const int registered = register_inst();
	};

	template <typename T>
	class ClassInfo2 {
	public:
		static const std::string_view get_name() { return name; }

		template <typename T>
		friend const std::string_view register_class(const std::string_view _v);
	private:
		static inline std::string_view name;
	};

	template <typename T>
	const std::string_view register_class(const std::string_view _v) {
		ClassInfo2<T>::name = _v;
		return _v;
	}

}

