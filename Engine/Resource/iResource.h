#pragma once
#include "Engine/Entity.h"

#include "Engine/Resource/define_Resource.h"

namespace ehw
{
	class iResource 
		: public Entity
		, public std::enable_shared_from_this<Entity>
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

		template <typename T>
		inline std::shared_ptr<T> shared_from_this_T();


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

	template<typename T>
	inline std::shared_ptr<T> iResource::shared_from_this_T()
	{
		static_assert(std::is_base_of_v<iResource, T>);
		return std::static_pointer_cast<T>(shared_from_this());
	}

}
