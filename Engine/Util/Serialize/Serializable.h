#pragma once

#include "../type_traits_Ex.h"
#include "../../define_Enum.h"
#include "../../define_Macro.h"

#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}
#include <iostream>
#include <cstdint>
#include <vector>

namespace ehw
{
	template <typename T>
	concept is_serializable = requires (std::ostream & _ofs, std::istream & _ifs, T _t)
	{
		{ _ofs << _t } -> type_traits_Ex::Decay_Derived_From<std::ostream>;
		{ _ifs >> _t } -> type_traits_Ex::Decay_Derived_From<std::istream>;
	};

	//다중상속 용도
	template <is_serializable T>
	class Serializable
	{
	protected:
		Serializable() {};
		virtual ~Serializable() {};

	public:
		//여러 형태로 저장한 클래스의 경우(바이너리 + json) 명시적으로 호출해줘야 함
		//Serializable<JsonSerializer>::SaveFile()
		//Serializable<BinarySerializer>::SaveFile()
		inline eResult SaveFile(std::fs::path const& _fullPath);
		inline eResult LoadFile(std::fs::path const& _fullPath);

		virtual eResult Serialize(T& _ser) = 0;
		virtual eResult DeSerialize(const T& _ser) = 0;
	};


	template <is_serializable T>
	inline eResult Serializable<T>::SaveFile(std::fs::path const& _fullPath)
	{
		std::fs::path parentDir = _fullPath.parent_path();

		//폴더 생성
		if (false == std::fs::exists(parentDir))
		{
			try
			{
				if (false == std::fs::create_directories(parentDir))
				{
					return eResult::Fail_Path;
				}
			}
			catch (const std::exception& _err)
			{
				ERROR_MESSAGE_A(err.what());
				return eResult::Fail_Path;
			}
		}

		//저장할 파일 오픈
		std::ofstream ofs(_fullPath, std::ios::binary);
		if (false == ofs.is_open())
		{
			ERROR_MESSAGE("파일 열기 실패");
			return eResult::Fail_Open;
		}

		//Serialize
		T ser{};
		eResult result = Serialize(&ser);
		if (eResultFail(result))
		{
			return result;
		}

		//파일 저장
		ofs << ser;
		ofs.close();

		return eResult::Success;
	}

	template<is_serializable T>
	inline eResult Serializable<T>::LoadFile(std::filesystem::path const& _fullPath)
	{
		if (false == std::fs::exists(_fullPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return eResult::Fail_Open;
		}
		
		//파일 오픈
		std::ifstream ifs(_fullPath, std::ios::binary);
		if (false == ifs.is_open())
		{
			ERROR_MESSAGE("파일을 열지 못했습니다.");
			return eResult::Fail_Open;
		}

		T ser{};
		ifs >> ser;
		ifs.close();

		return ser.DeSerialize();
	}
}
