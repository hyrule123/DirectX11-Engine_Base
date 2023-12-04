#include "PCH_Engine.h"
#include "IResource.h"

#include "PathMgr.h"

#include "json-cpp/json.h"

namespace ehw
{
	namespace strKey::Json::IResource
	{
		STRKEY_DECLARE(m_ComCategory);
	}

	

	IResource::IResource(eResourceType _type)
		: m_ComCategory(_type)
		, mbEngineDefaultRes()
	{
	}
	IResource::~IResource()
	{
	}

	eResult IResource::Save(const std::fs::path& _filePath)
	{
		SetKey(_filePath.string());
		return eResult::Success;
	}

	eResult IResource::Load(const std::fs::path& _filePath)
	{
		SetKey(_filePath.string());
		return eResult::Success;
	}

	//	return eResult::Success;
	//}
	eResult IResource::SaveJson(Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = Entity::SaveJson(_pJVal);
		
		if (eResult::Success != result)
		{
			return result;
		}

		Json::Value& Jval = *(_pJVal);

		//리소스 타입을 문자열 형태로 가져와서 저장
		Jval[strKey::Json::IResource::m_ComCategory] = strKey::ArrResName[(int)m_ComCategory];


		return eResult::Success;
	}
	eResult IResource::LoadJson(const Json::Value* _pJVal)
	{
		if (nullptr == _pJVal)
		{
			return eResult::Fail_Nullptr;
		}

		eResult result = Entity::LoadJson(_pJVal);
		if (eResult::Success != result)
		{
			return result;
		}

		const Json::Value& Jval = *(_pJVal);

		//저장된 json 파일과 로드할 json 파일의 타입이 일치하는지만 확인.
		if (Jval.isMember(strKey::Json::IResource::m_ComCategory))
		{
			std::string ResourceName = Jval[strKey::Json::IResource::m_ComCategory].asString();
			if (strKey::ArrResName[(int)m_ComCategory] != ResourceName)
			{
				ERROR_MESSAGE_W(L"읽어들인 json 파일이 리소스 타입과 일치하지 않습니다.");
				return eResult::Fail_InValid;
			}
		}


		return eResult::Success;
	}



}
