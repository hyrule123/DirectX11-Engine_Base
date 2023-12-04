#pragma once
#include "Entity.h"
#include "define_Res.h"
#include "PathMgr.h"
#include "json-cpp/json-forwards.h"

namespace ehw
{
	class IResource 
		: public Entity
	{
	public:
		IResource(eResourceType _type);

		IResource(const IResource& _other) = default;

		virtual ~IResource();

		//BasePath를 저장
		virtual eResult Save(const std::fs::path& _filePath);
		virtual eResult Load(const std::fs::path& _filePath);

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		eResourceType GetResType() { return m_ComCategory; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		const eResourceType m_ComCategory;
		bool mbEngineDefaultRes;
	};

}
