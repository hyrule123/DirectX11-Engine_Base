#pragma once
#include "Util/AtExit.h"
#include "CommonStruct.h"
#include "Resource/iResource.h"
#include "ResourceManagers.h"

#include <filesystem>
namespace stdfs = std::filesystem;

namespace ehw
{
	template <typename BaseResType>
	class tResourceManager
	{
		static_assert(std::is_base_of_v<iResource, BaseResType>);

	public:
		static void Init(const stdfs::path& _baseDir);

		template <typename DerivedResType = BaseResType>
		static std::shared_ptr<DerivedResType> Load(const stdfs::path& _filePath);

		static void Insert(const std::string_view _strKey, std::shared_ptr<BaseResType> _Res);

		template <typename DerivedResType = BaseResType>
		static std::shared_ptr<DerivedResType> Find(const std::string_view _strKey);

		static const std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>>&
			GetResources() { return m_Resources; }

		static void CleanUnusedResources();

		static void SetBaseDir(const stdfs::path& _baseDir) { m_BaseDir = _baseDir; }
		static const stdfs::path& GetBaseDir() { return m_BaseDir; }

	private:
		static void Release();

	private:
		static bool m_bInitialized;
		static stdfs::path m_BaseDir;

		static std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>> m_Resources;

	private:
		tResourceManager() = delete;
		~tResourceManager() = delete;
	};

	template <typename BaseResType>
	bool tResourceManager<BaseResType>::m_bInitialized = false;

	template <typename BaseResType>
	stdfs::path tResourceManager<BaseResType>::m_BaseDir{};

	template <typename BaseResType>
	std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>> tResourceManager<BaseResType>::m_Resources{};

	template<typename BaseResType>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> tResourceManager<BaseResType>::Load(const std::filesystem::path& _filePath)
	{
		static_assert(std::is_base_of_v<BaseResType, DerivedResType>);

		ASSERT(m_bInitialized, "초기화되지 않았습니다. Init()를 호출해주세요.");

		std::shared_ptr<DerivedResType> returnPtr = Find<DerivedResType>(_filePath.string());

		if(nullptr == returnPtr)
		{
			returnPtr = std::make_shared<DerivedResType>();

			eResult result = returnPtr->Load(_filePath);

			if (eResultSuccess(result))
			{
				Insert(_filePath.string(), static_pointer_cast<BaseResType>(returnPtr));
			}
			else
			{
				returnPtr = nullptr;
			}
		}

		return returnPtr;
	}



	template<typename BaseResType>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> tResourceManager<BaseResType>::Find(const std::string_view _strKey)
	{
		std::shared_ptr<DerivedResType> returnRes = nullptr;

		const auto& iter = m_Resources.find(_strKey);

		if (m_Resources.end() != iter)
		{
			//동일 리소스 타입일 경우 즉시 반환
			if constexpr (std::is_same_v<BaseResType, DerivedResType>)
			{
				returnRes = iter->second;
			}

			//다른 리소스 타입일 경우에는 타입 확인후 반환
			else
			{
				if (typeid(DerivedResType) == iter->second->GetResourceType())
				{
					returnRes = std::static_pointer_cast<DerivedResType>(iter->second);
				}
				else
				{
					returnRes = nullptr;
				}
			}
		}

		return returnRes;
	}

	template<typename BaseResType>
	inline void tResourceManager<BaseResType>::Insert(const std::string_view _strKey, std::shared_ptr<BaseResType> _Res)
	{
		ASSERT(nullptr == Find(_strKey), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->SetStrKey(_strKey);
		m_Resources.insert(std::make_pair(_strKey, _Res));
	}

	template<typename BaseResType>
	inline void tResourceManager<BaseResType>::Init(const stdfs::path& _baseDir)
	{
		m_bInitialized = true;

		SetBaseDir(_baseDir);

		ResourceManagers::AddUnusedResourceCleanFunc(CleanUnusedResources);

		AtExit::AddFunc(Release);
	}

	template<typename BaseResType>
	inline void tResourceManager<BaseResType>::Release()
	{
		m_bInitialized = false;
		m_BaseDir.clear();
		m_Resources.clear();
	}

	template<typename BaseResType>
	inline void tResourceManager<BaseResType>::CleanUnusedResources()
	{
		using pairType = decltype(m_Resources)::value_type;
		std::erase_if(m_Resources,
			[](const pairType& pair)->bool
			{
				bool needErase = false;
				if ((long)1 == pair.second.use_count())
				{
					needErase = true;
				}
				return needErase;
			}
		);
	}









}


