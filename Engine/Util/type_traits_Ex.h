#pragma once

#include <type_traits>
#include <concepts>
#include <vector>
#include <array>
#include <memory>
#include <string>

//Custom Is_V
namespace type_traits_Ex
{
	template <typename P>
	concept PointerTypes = requires(P p) { { *p }; }&&
		std::equality_comparable_with<std::nullptr_t, P>;

	template <typename P>
	concept NotPointerTypes = !PointerTypes<P>;

	template<typename T, typename CharType>
	concept BasicStringLike = std::is_convertible_v<T, std::basic_string_view<CharType>>;

	template <typename T>
	concept StringLike = BasicStringLike<T, char>;
	template <typename T>
	concept WStringLike = BasicStringLike<T, wchar_t>;

	template <class T> struct is_shared_ptr : std::false_type {};
	template <class T> struct is_shared_ptr<std::shared_ptr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_shared_ptr_v = is_shared_ptr<T>::value;

	template <class T> struct is_unique_ptr : std::false_type {};
	template <class T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_unique_ptr_v = is_unique_ptr<T>::value;

	template <class T> struct is_smart_ptr : std::false_type {};
	template <class T> struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};
	template <class T> struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};
	template <class T> inline constexpr bool is_smart_ptr_v = is_smart_ptr<T>::value;

	template <typename T>
	struct is_string : std::false_type {};

	template <typename T>
	struct is_vector : std::false_type {};
	template <typename T, typename Alloc>
	struct is_vector<std::vector<T, Alloc>> : std::true_type {};
	template <typename T>
	constexpr bool is_vector_v = is_vector<T>::value;

	// Base template for is_container (false by default)
	template <typename T, typename = void>
	struct is_container : std::false_type {};
	// Specializations for types that behave like containers
	template <typename T>
	struct is_container<T, std::void_t<
		typename T::value_type,
		decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end()),
		decltype(std::declval<T>().size())
		>> : std::true_type {};
	template <class T> inline constexpr bool is_container_v = is_container<T>::value;


	template <typename T, typename = void>
	struct is_pair_container : std::false_type {};
	template <typename T>
	struct is_pair_container<T, std::void_t<
		typename T::value_type,
		decltype(std::declval<typename T::value_type::first_type>()),
		decltype(std::declval<typename T::value_type::second_type>()),
		decltype(std::declval<T>().begin()),
		decltype(std::declval<T>().end()),
		decltype(std::declval<T>().size())
		>> : std::true_type{};
	template <class T> inline constexpr bool is_pair_container_v = is_pair_container<T>::value;


	template <typename T, typename = void>
	struct is_value_type_pointer : std::false_type {};

	template <typename T>
	struct is_value_type_pointer<T, std::enable_if_t<std::is_pointer_v<typename T::value_type> || is_smart_ptr_v<typename T::value_type>>> : std::true_type {};

	template <typename T> inline constexpr bool is_value_type_pointer_v = is_value_type_pointer<T>::value;

	template<class T>
	struct is_std_array : std::false_type {};
	template<class T, size_t N>
	struct is_std_array<std::array<T, N>> :std::true_type {};

	template <class T> inline constexpr bool is_std_array_v = is_std_array<T>::value;
}
