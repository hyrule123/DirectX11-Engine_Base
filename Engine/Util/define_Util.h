#pragma once
#include <Engine/Util/Serialize/base64.h>
#include <Engine/Util/type_traits_Ex.h>


namespace core
{
	//class BinaryWriter
	//{
	//public:

	//};
	//class Binary
	//{
	//public:
	//	template<typename T>
	//	static void SaveValue(std::ofstream& _ofs, const T& _val)
	//	{
	//		_ofs.write(reinterpret_cast<const char*>(&_val), sizeof(T));
	//	}

	//	template<typename T>
	//	static void LoadValue(std::ifstream& _ifs, T& _val)
	//	{
	//		_ifs.read(reinterpret_cast<char*>(&_val), sizeof(T));
	//	}
	//

	//	//Vector 또는 String
	//	template <typename T, typename std::enable_if_t<(
	//		type_traits_Ex::is_vector_v<T> ||
	//		std::is_same_v<std::string, T>
	//	)>* = nullptr>
	//	static void SaveValueVector(std::ofstream& _ofs, const T& _val)
	//	{
	//		using valType = T::value_type;
	//		size_t size = _val.size();
	//		_ofs.write(reinterpret_cast<const char*>(&size), sizeof(size_t));;
	//		_ofs.write(reinterpret_cast<const char*>(_val.data()), _val.size() * sizeof(valType));
	//	}

	//	template <typename T, typename std::enable_if_t<(
	//		type_traits_Ex::is_vector_v<T> ||
	//		std::is_same_v<std::string, T>
	//	)>* = nullptr>
	//		static void LoadValueVector(std::ifstream& _ifs, T& _val)
	//	{
	//		using valType = T::value_type;
	//		size_t size{};
	//		_ifs.read(reinterpret_cast<char*>(&size), sizeof(size_t));
	//		_val.resize(size);
	//		_ifs.read(reinterpret_cast<char*>(_val.data()), _val.size() * sizeof(valType));
	//	}

	//	static void SaveStr(std::ofstream& _ofs, const std::string& _val)
	//	{
	//		SaveValueVector(_ofs, _val);
	//	}

	//	static void LoadStr(std::ifstream& _ifs, std::string& _val)
	//	{
	//		LoadValueVector(_ifs, _val);
	//	}

	//private:
	//	Binary() = delete;
	//	~Binary() = delete;
	//};

	class WinAPI
	{
	public:
		static std::filesystem::path file_open_dialog(const std::filesystem::path& _baseDirectory, const std::filesystem::path& _extension);

		static std::filesystem::path file_open_dialog(const std::filesystem::path& _baseDirectory, const std::vector<std::filesystem::path>& _extensions);
	private:
		WinAPI() = delete;
		~WinAPI() = delete;
	};


	
}



