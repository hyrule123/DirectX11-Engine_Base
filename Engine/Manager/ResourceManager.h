#pragma once
#include "../Util/AtExit.h"
#include "../CommonStruct.h"
#include "../Resource/iResource.h"

#include "ResourceManagers.h"

#include <concepts>

#define LOAD_COMPUTESHADER(_type) ResourceManager<iComputeShader>::Load<_type>(#_type)

namespace ehw
{
	template <typename BaseResType> 
		requires std::is_base_of_v<iResource, BaseResType>
	class ResourceManager
	{
	public:
		static inline void Init(const std::fs::path& _baseDir);

		static inline eResult Save(const std::string_view _strKey);
		static inline eResult Save(const std::shared_ptr<BaseResType>& _resPtr);
		static inline eResult Save(const std::shared_ptr<BaseResType>& _resPtr, const std::fs::path& _strKeyPath);

		template <typename DerivedResType = BaseResType>
		static inline std::shared_ptr<DerivedResType> Load(const std::fs::path& _strKeyPath);

		template <typename DerivedResType = BaseResType>
		static inline void Insert(const std::string_view _strKey, std::shared_ptr<DerivedResType> _Res);

		template <typename DerivedResType = BaseResType>
		static inline std::shared_ptr<DerivedResType> Find(const std::string_view _strKey);

		static const std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>>&
			GetResources() { return m_Resources; }

		static inline std::vector<std::shared_ptr<iResource>> GetResourcesVector();

		static inline void CleanUnusedResources();

		static void SetBaseDir(const std::fs::path& _baseDir) { m_BaseDir = _baseDir; }
		static const std::fs::path& GetBaseDir() { return m_BaseDir; }

	private:
		static void Release();

	private:
		static bool m_bInitialized;
		static std::fs::path m_BaseDir;

		static std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>> m_Resources;

	private:
		ResourceManager() = delete;
		~ResourceManager() = delete;
	};

	template <typename BaseResType>
	bool ResourceManager<BaseResType>::m_bInitialized = false;

	template <typename BaseResType>
	std::fs::path ResourceManager<BaseResType>::m_BaseDir{};

	template <typename BaseResType>
	std::unordered_map<std::string, std::shared_ptr<BaseResType>, tHashFunc_StringView, std::equal_to<>> ResourceManager<BaseResType>::m_Resources{};

	template<typename BaseResType>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> ResourceManager<BaseResType>::Load(const std::filesystem::path& _strKeyPath)
	{
		static_assert(std::is_base_of_v<BaseResType, DerivedResType>);

		ASSERT(m_bInitialized, "초기화되지 않았습니다. Init()를 호출한 뒤 사용하세요.");

		std::shared_ptr<DerivedResType> returnPtr = Find<DerivedResType>(_strKeyPath.string());

		if(nullptr == returnPtr)
		{
			returnPtr = std::make_shared<DerivedResType>();

			returnPtr->SetStrKey(_strKeyPath.string());

			eResult result = returnPtr->Load(m_BaseDir, _strKeyPath);

			if (eResultSuccess(result))
			{
				Insert(_strKeyPath.string(), static_pointer_cast<BaseResType>(returnPtr));
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
	inline std::shared_ptr<DerivedResType> ResourceManager<BaseResType>::Find(const std::string_view _strKey)
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
	template <typename DerivedResType>
	inline void ResourceManager<BaseResType>::Insert(const std::string_view _strKey, std::shared_ptr<DerivedResType> _Res)
	{
		static_assert(std::is_base_of_v<BaseResType, DerivedResType>, "넣으려는 리소스 타입이 BaseResType의 상속 클래스가 아닙니다.");
		ASSERT(nullptr == Find(_strKey), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->SetStrKey(_strKey);

		m_Resources.insert(std::make_pair(std::string(_strKey), std::static_pointer_cast<BaseResType>(_Res)));
	}

	template<typename BaseResType>
	inline void ResourceManager<BaseResType>::Init(const std::fs::path& _baseDir)
	{
		m_bInitialized = true;

		SetBaseDir(_baseDir);

		ResourceManagers::AddUnusedResourceCleanFunc(CleanUnusedResources);

		AtExit::AddFunc(Release);
	}

	template<typename BaseResType>
	inline eResult ResourceManager<BaseResType>::Save(const std::string_view _strKey)
	{
		const std::shared_ptr<BaseResType>& savedFile = Find(_strKey);
		if (nullptr == savedFile)
		{
			return eResult::Fail_Nullptr;
		}

		
		return savedFile->Save(m_BaseDir, _strKey);
	}

	template<typename BaseResType>
	inline eResult ResourceManager<BaseResType>::Save(const std::shared_ptr<BaseResType>& _resPtr)
	{
		if (nullptr == _resPtr)
		{
			return eResult::Fail_Nullptr;
		}
		else if (_resPtr->GetStrKey().empty())
		{
			return eResult::Fail_InValid;
		}

		return _resPtr->Save(m_BaseDir, _resPtr->GetStrKey());
	}

	template<typename BaseResType>
	inline eResult ResourceManager<BaseResType>::Save(const std::shared_ptr<BaseResType>& _resPtr, const std::fs::path& _strKeyPath)
	{
		if (nullptr == _resPtr)
		{
			return eResult::Fail_Nullptr;
		}
		else if (_strKeyPath.empty())
		{
			return eResult::Fail_InValid;
		}

		//기존 키값 임시 저장
		std::string tempStr = _resPtr->GetStrKey();
		_resPtr->SetStrKey(_strKeyPath);

		//저장하고
		eResult result = _resPtr->Save(m_BaseDir, _strKeyPath);

		//원래 키값을 복원
		_resPtr->SetStrKey(tempStr);

		return result;
	}

	template<typename BaseResType>
	inline void ResourceManager<BaseResType>::Release()
	{
		m_bInitialized = false;
		m_BaseDir.clear();
		m_Resources.clear();
	}

	template<typename BaseResType>
	inline std::vector<std::shared_ptr<iResource>> ResourceManager<BaseResType>::GetResourcesVector()
	{
		std::vector<std::shared_ptr<iResource>> retVec{};

		for (const auto& iter : m_Resources)
		{
			retVec.emplace_back(std::static_pointer_cast<iResource>(iter.second));
		}

		return retVec;
	}

	template<typename BaseResType>
	inline void ResourceManager<BaseResType>::CleanUnusedResources()
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


