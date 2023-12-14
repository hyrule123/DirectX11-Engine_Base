#pragma once
#include "Util/AtExit.h"
#include "CommonStruct.h"
#include "Resource/iResource.h"
#include "ResourceManagers.h"

namespace ehw
{
	template <typename ResourceType>
	class tResourceManager
	{
	public:
		static std::shared_ptr<ResourceType> Load(const std::filesystem::path& _filePath);

		static void Insert(const std::string_view _strKey, std::shared_ptr<ResourceType> _Res);

		static std::shared_ptr<ResourceType> Find(const std::string_view _strKey);

		static const std::unordered_map<std::string, std::shared_ptr<ResourceType>, tHashFunc_StringView, std::equal_to<>>&
			GetResources() { return m_Resources; }


	private:
		static void Init();
		static void Release();


	private:
		static bool m_bInitialized;
		static std::string m_RelativePath;

		static std::unordered_map<std::string, std::shared_ptr<ResourceType>, tHashFunc_StringView, std::equal_to<>> m_Resources;

	private:
		tResourceManager() = delete;
		~tResourceManager() = delete;
	};

	template <typename ResourceType>
	bool tResourceManager<ResourceType>::m_bInitialized = false;

	template <typename ResourceType>
	std::string tResourceManager<ResourceType>::m_RelativePath{};

	template <typename ResourceType>
	std::unordered_map<std::string, std::shared_ptr<ResourceType>, tHashFunc_StringView, std::equal_to<>> tResourceManager<ResourceType>::m_Resources{};


	template<typename ResourceType>
	inline std::shared_ptr<ResourceType> tResourceManager<ResourceType>::Load(const std::filesystem::path& _filePath)
	{
		static_assert(std::is_base_of_v<iResource, ResourceType>);

		if (false == m_bInitialized)
		{
			Init();
		}

		std::shared_ptr<ResourceType> returnPtr = Find(_filePath.string());

		if(nullptr == returnPtr)
		{
			returnPtr = std::make_shared<ResourceType>();
			eResult result = returnPtr->Load(_filePath);

			if (eResultSuccess(result))
			{
				Insert(_filePath.string(), returnPtr);
			}
			else
			{
				returnPtr = nullptr;
			}
		}

		return returnPtr;
	}

	template<typename ResourceType>
	inline std::shared_ptr<ResourceType> tResourceManager<ResourceType>::Find(const std::string_view _strKey)
	{
		std::shared_ptr<ResourceType> returnRes = nullptr;

		const auto& iter = m_Resources.find(_strKey);
		if (m_Resources.end() != iter)
		{
			returnRes = iter->second;
		}

		return returnRes;
	}

	template<typename ResourceType>
	inline void tResourceManager<ResourceType>::Insert(const std::string_view _strKey, std::shared_ptr<ResourceType> _Res)
	{
		ASSERT(nullptr == Find(_strKey), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->SetStrKey(_strKey);
		m_Resources.insert(std::make_pair(_strKey, _Res));
	}

	template<typename ResourceType>
	inline void tResourceManager<ResourceType>::Init()
	{
		m_bInitialized = true;

		AtExit::AddFunc(Release);
	}

	template<typename ResourceType>
	inline void tResourceManager<ResourceType>::Release()
	{
		m_Resources.clear();
	}

}


