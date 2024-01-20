#pragma once

#include "../type_traits_Ex.h"
#include "../../define_Macro.h"
#include "../../define_Enum.h"
#include "../../CommonType.h"

#include <filesystem>
#include <iostream>
#include <cstdint>
#include <vector>
#include <concepts>
#include <string>

namespace ehw
{
	template <typename T>
	struct Remove_Const_Reference
	{
		using type = std::remove_const_t<std::remove_reference_t<T>>;
	};
	template <typename T>
	using Remove_Const_Reference_t = Remove_Const_Reference<T>::type;

	template <typename T>
	concept PointerTypes = requires(T a) { *a; };
	template <typename T>
	concept NotPointerTypes = !requires(T a) { *a; };

	template <typename T>
	concept AllowedTypes =
		//* 연산자가 있을경우(포인터일경우) 저장 불가
		!PointerTypes<T> &&
		//string이 아니여야 함
		!(std::is_same_v<Remove_Const_Reference_t<T>, std::string> ||
			std::is_same_v<Remove_Const_Reference_t<T>, std::wstring>);

	template <typename T>
	concept StringTypes =
		std::is_same_v<T, std::string> ||
		std::is_same_v<T, std::wstring>;


	class Serializer
	{
	protected:
		Serializer() {};
		virtual ~Serializer() {};
	public:
		virtual eResult SaveFile(std::filesystem::path const& _fullPath) = 0;
		virtual eResult LoadFile(std::filesystem::path const& _fullPath) = 0;
	};


	//다중상속 용도
	template <typename T> requires std::is_base_of_v<Serializer, T>
	class Serializable
	{
	protected:
		Serializable() {};
		virtual ~Serializable() {};

	public:
		inline eResult SaveFile(std::filesystem::path const& _fullPath);
		inline eResult LoadFile(std::filesystem::path const& _fullPath);

		virtual eResult Serialize(T& _ser) = 0;
		virtual eResult DeSerialize(T& _ser) = 0;
	};


	template<typename T>
	inline eResult Serializable<T>::SaveFile(std::filesystem::path const& _fullPath)
	{
		T ser{};

		bool result = Serialize(&ser);
		if (eResultFail(result))
		{
			return false;
		}

		return ser.SaveFile(_fullPath);
	}

	template<typename T>
	inline eResult Serializable<T>::LoadFile(std::filesystem::path const& _fullPath)
	{
		T ser{};
		eResult result = ser.LoadFile(_fullPath);

		if (eResultFail(result))
		{
			return false;
		}

		return ser.DeSerialize();
	}
}


