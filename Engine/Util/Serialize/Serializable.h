#pragma once
#include "Engine/define_Enum.h"
#include "Engine/define_Macro.h"

#include "Engine/Util/type_traits_Ex.h"

#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}
#include <fstream>
#include <string>

//다중상속용 Interface Class
//===================================================================================================
//사용법
// 데이터 직렬화가 필요한 클래스의 헤더에서 이 헤더를 include 한다. 
// Serializable_Json 또는 Serializable_Binary를 다중 상속받는다.
// CPP 파일에서 직렬화 타입에 따라 BinarySerializer 또는 JsonSerializer 헤더를 include 한다.
// --> 이 헤더 파일에는 전방선언만 되어 있음.
// 가상함수 Serialize_OOO 함수를 재정의해서 해당 클래스 직렬화 코드를 작성한다.
// Serialize 함수를 재정의할 떄 반드시 포인터 체크를 해줄것.
//====================================================================================================

#define SERIALIZER_CHECK_PTR(_ser) if(nullptr == _ser) { ERROR_MESSAGE("Serializer가 nullptr 이었습니다."); return eResult::Fail_Nullptr; }

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

		eResult SaveFile_Binary(std::fs::path const& _fullPath) const;
		eResult LoadFile_Binary(std::fs::path const& _fullPath);
		
		virtual eResult Serialize_Binary(BinarySerializer* _ser) const = 0;
		virtual eResult DeSerialize_Binary(const BinarySerializer* _ser) = 0;
	};
#pragma endregion //BINARY
}

#pragma region JSON
#include "json-forwards.h"
#define JSON_KEY(_varName) #_varName

//Json의 경우 try-catch문 안에 넣어줄것(Json 라이브러리가 해당 방식으로 에러 핸들링을 하고있음)
namespace ehw
{
	using JsonSerializer = Json::Value;
	class Serializable_Json
		: public Serializable
	{
	public:
		Serializable_Json();
		virtual ~Serializable_Json();

		eResult SaveFile_Json(std::fs::path const& _fullPath) const;
		eResult LoadFile_Json(std::fs::path const& _fullPath);

		virtual eResult serialize_json(JsonSerializer* _ser) const = 0;
		virtual eResult deserialize_json(const JsonSerializer* _ser) = 0;
	};
#pragma endregion //JSON
}
