#include "Engine/Util/Serialize/Serializable.h"

#include "BinarySerializer.h"
#include "JsonSerializer.h"

namespace core
{
	Serializable::Serializable()
	{
	}
	Serializable::~Serializable()
	{
	}
	s_ptr<std::ofstream> Serializable::OpenOfstream(std::fs::path const& _fullPath, std::ios::openmode _mode) const
	{
		std::fs::path parentDir = _fullPath.parent_path();

		//폴더 생성
		if (false == std::fs::exists(parentDir))
		{
			try
			{
				if (false == std::fs::create_directories(parentDir))
				{
					return nullptr;
				}
			}
			catch (const std::exception& _err)
			{
				ERROR_MESSAGE_A(_err.what());
				return nullptr;
			}
		}

		//저장할 파일 오픈
		s_ptr<std::ofstream> ofs = std::make_shared<std::ofstream>(_fullPath, _mode);
		if (false == ofs->is_open())
		{
			ERROR_MESSAGE("파일 열기 실패");
			return nullptr;
		}

		return ofs;
	}

	s_ptr<std::ifstream> Serializable::OpenIfstream(std::fs::path const& _fullPath, std::ios::openmode _mode) const
	{
		if (false == std::fs::exists(_fullPath))
		{
			ERROR_MESSAGE("파일을 찾지 못했습니다.");
			return nullptr;
		}

		//파일 오픈
		s_ptr<std::ifstream> ifs = std::make_shared<std::ifstream>(_fullPath, std::ios::binary);
		if (false == ifs->is_open())
		{
			ERROR_MESSAGE("파일을 열지 못했습니다.");
			return nullptr;
		}

		return ifs;
	}

	Serializable_Binary::Serializable_Binary()
	{
	}
	Serializable_Binary::~Serializable_Binary()
	{
	}
	eResult Serializable_Binary::SaveFile_Binary(std::fs::path const& _fullPath) const
	{
		s_ptr<std::ofstream> ofs = OpenOfstream(_fullPath);

		if (nullptr == ofs)
		{
			return eResult::Fail_Nullptr;
		}

		//Serialize
		BinarySerializer ser{};
		eResult result = serialize_binary(&ser);
		if (eResult_fail(result))
		{
			return result;
		}

		//파일 저장
		(*ofs) << ser;
		ofs->close();

		return eResult::Success;
	}

	eResult Serializable_Binary::LoadFile_Binary(std::fs::path const& _fullPath)
	{
		s_ptr<std::ifstream> ifs = OpenIfstream(_fullPath);
		if (nullptr == ifs)
		{
			return eResult::Fail_Nullptr;
		}

		BinarySerializer ser{};
		(*ifs) >> ser;
		ifs->close();

		return deserialize_binary(&ser);
	}



	Serializable_Json::Serializable_Json()
	{
	}

	Serializable_Json::~Serializable_Json()
	{
	}

	eResult Serializable_Json::SaveFile_Json(std::fs::path const& _fullPath) const
	{
		s_ptr<std::ofstream> ofs = OpenOfstream(_fullPath);

		if (nullptr == ofs)
		{
			return eResult::Fail_Nullptr;
		}

		//Serialize
		Json::Value ser{};
		eResult result = serialize_json(&ser);
		if (eResult_fail(result))
		{
			return result;
		}

		//파일 저장
		(*ofs) << ser;
		ofs->close();

		return eResult::Success;
	}

	eResult Serializable_Json::LoadFile_Json(std::fs::path const& _fullPath)
	{
		s_ptr<std::ifstream> ifs = OpenIfstream(_fullPath);
		if (nullptr == ifs)
		{
			return eResult::Fail_Nullptr;
		}

		Json::Value ser{};
		
		try
		{
			(*ifs) >> ser;
			ifs->close();	
		}
		catch (const std::exception& _err)
		{
			ERROR_MESSAGE_A(_err.what());
			return eResult::Fail_Open;
		}


		return deserialize_json(&ser);
	}
}

