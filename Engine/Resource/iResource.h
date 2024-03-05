#pragma once
#include "Entity.h"

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

		//baseDir = 해당 리소스 모음 폴더까지의 경로. ex: ....Res/Texture
		//strKeyPath = 이후 경로. ex: Player/Player.png -> 리소스를 찾는 키값으로 사용됨
		virtual eResult Save(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) const;
		virtual eResult Load(const std::fs::path& _baseDir, const std::fs::path& _strKeyPath) = 0;

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
		return (typeid(ResourceType) == m_ResourceTypeInfo);
	}

}
