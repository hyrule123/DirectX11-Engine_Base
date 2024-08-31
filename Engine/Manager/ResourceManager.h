#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Util/StaticSingleton.h"

#include "ResourceManagers.h"

#include <concepts>

#define LOAD_COMPUTESHADER(_type) ResourceManager<ComputeShader>::GetInst().Load<_type>(#_type)

namespace ehw
{
	template <typename T>
	concept ResourceTypes = std::is_base_of_v<Resource, T>;

	template <typename ResourceTypes> 
	class ResourceManager : public StaticSingleton<ResourceManager<ResourceTypes>>
	{
		friend class StaticSingleton<ResourceManager<ResourceTypes>>;

	public:
		void Init(const std::fs::path& _baseDir);
		void Release();

		inline eResult Save(const std::string_view _strKey);
		inline eResult Save(ResourceTypes* _resPtr);
		inline eResult Save(ResourceTypes* _resPtr, const std::fs::path& _strKeyPath);

		template <typename DerivedResType = ResourceTypes>
		inline std::shared_ptr<DerivedResType> Load(const std::fs::path& _strKeyPath);

		template <typename DerivedResType = ResourceTypes>
		inline void Insert(const std::string_view _strKey, std::shared_ptr<DerivedResType> _Res);

		template <typename DerivedResType = ResourceTypes>
		inline std::shared_ptr<DerivedResType> Find(const std::string_view _strKey);

		const std::unordered_map<std::string, std::shared_ptr<ResourceTypes>, tHasher_StringView, std::equal_to<>>&
			GetResources() { return m_Resources; }

		inline std::vector<std::shared_ptr<Resource>> GetResourcesVector();

		inline void CleanUnusedResources();

		inline void SetBaseDir(const std::fs::path& _baseDir);
		const std::fs::path& GetBaseDir() { return m_BaseDir; }

	private:
		ResourceManager();
		~ResourceManager();

	private:
		bool m_bInitialized;
		std::fs::path m_BaseDir;

		std::unordered_map<std::string, std::shared_ptr<ResourceTypes>, tHasher_StringView, std::equal_to<>> m_Resources;
	};

	template<typename ResourceTypes>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> ResourceManager<ResourceTypes>::Load(const std::filesystem::path& _strKeyPath)
	{
		static_assert(std::is_base_of_v<ResourceTypes, DerivedResType>);

		ASSERT(m_bInitialized, "초기화되지 않았습니다. Init()를 호출한 뒤 사용하세요.");

		if (_strKeyPath.empty())
		{
			return nullptr;
		}

		std::shared_ptr<DerivedResType> returnPtr = Find<DerivedResType>(_strKeyPath.string());

		if(nullptr == returnPtr)
		{
			returnPtr = std::make_shared<DerivedResType>();

			returnPtr->SetStrKey(_strKeyPath.string());

			eResult result = returnPtr->Load(m_BaseDir, _strKeyPath);

			if (eResult_success(result))
			{
				Insert(_strKeyPath.string(), static_pointer_cast<ResourceTypes>(returnPtr));
			}
			else
			{
				returnPtr = nullptr;
			}
		}

		return returnPtr;
	}



	template<typename ResourceTypes>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> ResourceManager<ResourceTypes>::Find(const std::string_view _strKey)
	{
		std::shared_ptr<DerivedResType> returnRes = nullptr;

		const auto& iter = m_Resources.find(_strKey);

		if (m_Resources.end() != iter)
		{
			//동일 리소스 타입일 경우 즉시 반환
			if constexpr (std::is_same_v<ResourceTypes, DerivedResType>)
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

	template<typename ResourceTypes>
	template <typename DerivedResType>
	inline void ResourceManager<ResourceTypes>::Insert(const std::string_view _strKey, std::shared_ptr<DerivedResType> _Res)
	{
		static_assert(std::is_base_of_v<ResourceTypes, DerivedResType>, "넣으려는 리소스 타입이 ResourceTypes의 상속 클래스가 아닙니다.");
		ASSERT(nullptr == Find(_strKey), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->SetStrKey(_strKey);

		m_Resources.insert(std::make_pair(std::string(_strKey), std::static_pointer_cast<ResourceTypes>(_Res)));
	}

	template<typename ResourceTypes>
	inline ResourceManager<ResourceTypes>::ResourceManager()
		: m_bInitialized{false}
		, m_BaseDir{}
		, m_Resources{}
	{
	}

	template<typename ResourceTypes>
	inline ResourceManager<ResourceTypes>::~ResourceManager()
	{
	}

	template<typename ResourceTypes>
	inline void ResourceManager<ResourceTypes>::Init(const std::fs::path& _baseDir)
	{
		m_bInitialized = true;

		SetBaseDir(_baseDir);

		ResourceManagers::GetInst().AddUnusedResourceCleanFunc(std::bind(&ResourceManager<ResourceTypes>::CleanUnusedResources, this));

		AtExit::AddFunc(std::bind(&ResourceManager<ResourceTypes>::Release, this));
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::Save(const std::string_view _strKey)
	{
		const std::shared_ptr<ResourceTypes>& savedFile = Find(_strKey);
		if (nullptr == savedFile)
		{
			return eResult::Fail_Nullptr;
		}
		
		return savedFile->Save(m_BaseDir, _strKey);
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::Save(ResourceTypes* _resPtr)
	{
		if (nullptr == _resPtr)
		{
			return eResult::Fail_Nullptr;
		}
		else if (_resPtr->get_strkey().empty())
		{
			return eResult::Fail_InValid;
		}

		return _resPtr->Save(m_BaseDir, _resPtr->get_strkey());
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::Save(ResourceTypes* _resPtr, const std::fs::path& _strKeyPath)
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
		std::string tempStr{ _resPtr->get_strkey() };
		_resPtr->SetStrKey(_strKeyPath.string());

		//저장하고
		eResult result = _resPtr->Save(m_BaseDir, _strKeyPath);

		//원래 키값을 복원
		_resPtr->SetStrKey(tempStr);

		return result;
	}

	template<typename ResourceTypes>
	inline void ResourceManager<ResourceTypes>::Release()
	{
		m_bInitialized = false;
		m_BaseDir.clear();
		m_Resources.clear();
	}

	template<typename ResourceTypes>
	inline std::vector<std::shared_ptr<Resource>> ResourceManager<ResourceTypes>::GetResourcesVector()
	{
		std::vector<std::shared_ptr<Resource>> retVec{};

		for (const auto& iter : m_Resources)
		{
			retVec.emplace_back(std::static_pointer_cast<Resource>(iter.second));
		}

		return retVec;
	}

	template<typename ResourceTypes>
	inline void ResourceManager<ResourceTypes>::CleanUnusedResources()
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

	template<typename ResourceTypes>
	inline void ResourceManager<ResourceTypes>::SetBaseDir(const std::fs::path& _baseDir)
	{
		m_BaseDir = _baseDir;

		if (false == std::fs::exists(m_BaseDir))
		{
			bool result = std::fs::create_directories(m_BaseDir);
			ASSERT(result, "리소스 기본 디렉토리 생성에 실패했습니다.");
		}
	}









}


