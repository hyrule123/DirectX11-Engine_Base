#pragma once
#include "Entity.h"
#include "define_Res.h"
#include "PathManager.h"
#include "json-cpp/json-forwards.h"

namespace ehw
{
	class iResource 
		: public Entity
	{
	public:
		iResource(eResourceType _type);

		iResource(const iResource& _other) = default;

		virtual ~iResource();

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
