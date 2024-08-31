#pragma once
#include "Engine/Manager/InstanceManager.h"

#include <type_traits>

#define INSTANCE(_class_) InstanceRegisterHelper<_class_, #_class_>
#define REGISTER_INSTANCE(_class_) InstanceRegisterHelper<_class_, #_class_>::get_strkey()

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
	class Instance {
	public:
		inline static const std::string_view get_strkey() { return key; }
	protected:
		static std::string_view key;
	};

	//최초 선언 시 타입에 대한 문자열을 등록한다.
	template <typename T, FixedString S>
	class InstanceRegisterHelper
		: public Instance<T>
	{
	private:
		static const int register_inst() {
			Instance<T>::key = std::string_view(S);
			InstanceManager::GetInst().add_ctor<T>(Instance<T>::key);
			return 1;
		}
		//처음 1회 초기화를 위한 변수
		static inline const int registered = register_inst();
	};
}

