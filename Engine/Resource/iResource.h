#pragma once
#include "../Entity.h"


#include "../json-cpp/json-forwards.h"


#include "define_Resource.h"

namespace ehw
{
	class iResource 
		: public Entity
	{
	public:
		iResource(const std::type_info& _info);

		iResource(const iResource& _other) = default;

		virtual ~iResource();

		//BasePath를 저장
		virtual eResult Save(const std::fs::path& _pathFromBaseDir);
		virtual eResult Load(const std::fs::path& _pathFromBaseDir);

		virtual eResult SaveJson(Json::Value* _pJVal) override;
		virtual eResult LoadJson(const Json::Value* _pJVal) override;

		const std::type_info& GetResourceType() { return m_ResourceTypeInfo; }

		void SetEngineDefaultRes(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

		template <class ResourceType>
		inline bool Is();

	private:
		const std::type_info& m_ResourceTypeInfo;
		bool mbEngineDefaultRes;
	};

	template<class ResourceType>
	inline bool iResource::Is()
	{
		return (m_ResourceTypeInfo == typeid(ResourceType));
	}

}
