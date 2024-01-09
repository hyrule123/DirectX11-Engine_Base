#pragma once
#include "base64.h"

#include <fstream>
#include <filesystem>
#include <vector>


//Custom Is_V
namespace std
{
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
	struct is_value_type_pointer<T, std::enable_if_t<std::is_pointer_v<typename T::value_type> || std::is_smart_ptr_v<typename T::value_type>>> : std::true_type {};

	template <typename T> inline constexpr bool is_value_type_pointer_v = is_value_type_pointer<T>::value;

	template<class T>
	struct is_std_array : false_type {};
	template<class T, size_t N>
	struct is_std_array<array<T, N>> :true_type {};

	template <class T> inline constexpr bool is_std_array_v = is_std_array<T>::value;
}


namespace ehw
{
	class Binary
	{
	public:
		template<typename T>
		static void SaveValue(std::ofstream& _ofs, const T& _val)
		{
			_ofs.write(reinterpret_cast<const char*>(&_val), sizeof(T));
		}

		template<typename T>
		static void LoadValue(std::ifstream& _ifs, T& _val)
		{
			_ifs.read(reinterpret_cast<char*>(&_val), sizeof(T));
		}
	

		//Vector 또는 String
		template <typename T, typename std::enable_if_t<(
			std::is_vector_v<T> ||
			std::is_same_v<std::string, T>
		)>* = nullptr>
		static void SaveValueVector(std::ofstream& _ofs, const T& _val)
		{
			using valType = T::value_type;
			size_t size = _val.size();
			_ofs.write(reinterpret_cast<const char*>(&size), sizeof(size_t));;
			_ofs.write(reinterpret_cast<const char*>(_val.data()), _val.size() * sizeof(valType));
		}

		template <typename T, typename std::enable_if_t<(
			std::is_vector_v<T> ||
			std::is_same_v<std::string, T>
		)>* = nullptr>
			static void LoadValueVector(std::ifstream& _ifs, T& _val)
		{
			using valType = T::value_type;
			size_t size{};
			_ifs.read(reinterpret_cast<char*>(&size), sizeof(size_t));
			_val.resize(size);
			_ifs.read(reinterpret_cast<char*>(_val.data()), _val.size() * sizeof(valType));
		}

		static void SaveStr(std::ofstream& _ofs, const std::string& _val)
		{
			SaveValueVector(_ofs, _val);
		}

		static void LoadStr(std::ifstream& _ifs, std::string& _val)
		{
			LoadValueVector(_ifs, _val);
		}

	private:
		Binary() = delete;
		~Binary() = delete;
	};

	class WinAPI
	{
	public:
		static std::filesystem::path FileDialog(const std::filesystem::path& _baseDirectory, const std::filesystem::path& _extension);

		static std::filesystem::path FileDialog(const std::filesystem::path& _baseDirectory, const std::vector<std::filesystem::path>& _extensions);
	private:
		WinAPI() = delete;
		~WinAPI() = delete;
	};


	
}



