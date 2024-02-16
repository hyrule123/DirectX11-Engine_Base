#pragma once

#include "../type_traits_Ex.h"
#include "../../define_Enum.h"
#include "../../define_Macro.h"

#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}
#include <fstream>
#include <string>

//===================================================================================================
//사용법
// 데이터 직렬화가 필요한 클래스의 헤더에서 이 헤더를 include 한다. 
// Serializable_Json 또는 Serializable_Binary를 다중 상속받는다.
// CPP 파일에서 직렬화 타입에 따라 BinarySerializer 또는 JsonSerializer 헤더를 include 한다.
// --> 이 헤더 파일에는 전방선언만 되어 있음.
// 가상함수 Serialize_OOO 함수를 재정의해서 해당 클래스 직렬화 코드를 작성한다.
//====================================================================================================

namespace ehw
{
	class Serializable
	{
	protected:
		Serializable();
		virtual ~Serializable();

		std::shared_ptr<std::ofstream> OpenOfstream(std::fs::path const& _fullPath, std::ios::openmode _mode = std::ios::binary) const;
		std::shared_ptr<std::ifstream> OpenIfstream(std::fs::path const& _fullPath, std::ios::openmode _mode = std::ios::binary) const;
	};


#pragma region BINARY
	class BinarySerializer;
	class Serializable_Binary
		: public Serializable
	{
	public:
		Serializable_Binary();
		virtual ~Serializable_Binary();

		eResult SaveFile_Binary(std::fs::path const& _fullPath);
		eResult LoadFile_Binary(std::fs::path const& _fullPath);

		virtual eResult Serialize_Binary(BinarySerializer* _ser) = 0;
		virtual eResult DeSerialize_Binary(const BinarySerializer* _ser) = 0;
	};
#pragma endregion //BINARY
}

#pragma region JSON
#include "json-forwards.h"
#define JSON_KEY(_varName) #_varName

namespace ehw
{
	using JsonSerializer = Json::Value;
	class Serializable_Json
		: public Serializable
	{
	public:
		Serializable_Json();
		virtual ~Serializable_Json();

		eResult SaveFile_Json(std::fs::path const& _fullPath);
		eResult LoadFile_Json(std::fs::path const& _fullPath);

		virtual eResult Serialize_Json(JsonSerializer* _ser) = 0;
		virtual eResult DeSerialize_Json(const JsonSerializer* _ser) = 0;
	};
#pragma endregion //JSON
}
