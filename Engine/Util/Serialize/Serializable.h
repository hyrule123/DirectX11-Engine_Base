#pragma once

#include "json-forwards.h"
#include "../type_traits_Ex.h"
#include "../../define_Enum.h"
#include "../../define_Macro.h"

#include <filesystem>
namespace std
{
	namespace fs = filesystem;
}
#include <fstream>

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

		eResult SaveFileToBinary(std::fs::path const& _fullPath);
		eResult LoadFileFromBinary(std::fs::path const& _fullPath);

		virtual eResult Serialize_Binary(BinarySerializer* _ser) = 0;
		virtual eResult DeSerialize_Binary(const BinarySerializer* _ser) = 0;
	};
#pragma endregion //BINARY

#pragma region JSON
	class Serializable_Json
		: public Serializable
	{
	public:
		Serializable_Json();
		virtual ~Serializable_Json();

		eResult SaveFileToJson(std::fs::path const& _fullPath);
		eResult LoadFileFromJson(std::fs::path const& _fullPath);

		virtual eResult Serialize_Json(Json::Value* _ser) = 0;
		virtual eResult DeSerialize_Json(const Json::Value* _ser) = 0;
	};
#pragma endregion //JSON
}
