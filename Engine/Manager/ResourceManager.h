#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Util/StaticSingleton.h"

#include "ResourceManagers.h"

#include <concepts>

#define LOAD_COMPUTESHADER(_type) ResourceManager<ComputeShader>::get_inst().load_from_file<_type>(#_type)

namespace ehw
{
	template <typename T>
	concept ResourceTypes =
		std::is_base_of_v<Resource, T>
		&&
		IsBaseResource<T>::is;

	template <typename ResourceTypes> 
	class ResourceManager : public StaticSingleton<ResourceManager<ResourceTypes>>
	{
		friend class StaticSingleton<ResourceManager<ResourceTypes>>;
	public:
		using Resources = 
			std::unordered_map<std::string, std::shared_ptr<ResourceTypes>, Hasher_StringView, std::equal_to<>>;

		void init(const std::fs::path& _base_directory);
		void release();

		eResult save_to_file(const std::string_view _resource_name);
		eResult save_to_file(ResourceTypes* _resPtr);
		eResult save_to_file(ResourceTypes* _resPtr, const std::fs::path& _resource_name);

		template <typename DerivedResType = ResourceTypes>
		std::shared_ptr<DerivedResType> load_from_file(const std::fs::path& _resource_name, const std::fs::path& _base_path = {});

		template <typename DerivedResType = ResourceTypes>
		void insert(const std::string_view _resource_name, std::shared_ptr<DerivedResType> _Res);

		template <typename DerivedResType = ResourceTypes>
		std::shared_ptr<DerivedResType> find(const std::string_view _resource_name);

		const Resources& GetResources() { return m_Resources; }

		std::vector<std::shared_ptr<Resource>> GetResourcesVector();

		void CleanUnusedResources();

		void SetBaseDir(const std::fs::path& _base_directory);
		const std::fs::path& GetBaseDir() { return m_BaseDir; }

	private:
		ResourceManager();
		~ResourceManager();

	private:
		bool m_bInitialized;
		std::fs::path m_BaseDir;

		Resources m_Resources;
	};

	template<typename ResourceTypes>
	template<typename DerivedResType>
	inline std::shared_ptr<DerivedResType> ResourceManager<ResourceTypes>::load_from_file(const std::filesystem::path& _resource_name, const std::fs::path& _base_path)
	{
		static_assert(std::is_base_of_v<ResourceTypes, DerivedResType>);

		ASSERT(m_bInitialized, "초기화되지 않았습니다. init()를 호출한 뒤 사용하세요.");

		if (_resource_name.empty())
		{
			return nullptr;
		}

		std::string key_path = _resource_name.string();
		std::shared_ptr<DerivedResType> returnPtr = find<DerivedResType>(key_path);

		if(nullptr == returnPtr)
		{
			returnPtr = std::make_shared<DerivedResType>();
			eResult result{};
			if (_base_path.empty()) {
				result = returnPtr->load_from_file(m_BaseDir, _resource_name);
			}
			else {
				result = returnPtr->load_from_file(_base_path, _resource_name);
			}
			

			if (eResult_success(result))
			{
				insert(key_path, std::static_pointer_cast<ResourceTypes>(returnPtr));
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
	inline std::shared_ptr<DerivedResType> ResourceManager<ResourceTypes>::find(const std::string_view _resource_name)
	{
		std::shared_ptr<DerivedResType> returnRes = nullptr;

		const auto& iter = m_Resources.find(_resource_name);

		if (m_Resources.end() != iter)
		{
			returnRes = std::dynamic_pointer_cast<DerivedResType>(iter->second);
		}

		return returnRes;
	}

	template<typename ResourceTypes>
	template <typename DerivedResType>
	inline void ResourceManager<ResourceTypes>::insert(const std::string_view _resource_name, std::shared_ptr<DerivedResType> _Res)
	{
		static_assert(std::is_base_of_v<ResourceTypes, DerivedResType>, "넣으려는 리소스 타입이 ResourceTypes의 상속 클래스가 아닙니다.");
		ASSERT(nullptr == find(_resource_name), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->set_resource_name(_resource_name);

		m_Resources.insert(std::make_pair(std::string(_resource_name), std::static_pointer_cast<ResourceTypes>(_Res)));
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
	inline void ResourceManager<ResourceTypes>::init(const std::fs::path& _base_directory)
	{
		m_bInitialized = true;

		SetBaseDir(_base_directory);

		ResourceManagers::get_inst().AddUnusedResourceCleanFunc(std::bind(&ResourceManager<ResourceTypes>::CleanUnusedResources, this));

		AtExit::AddFunc(std::bind(&ResourceManager<ResourceTypes>::release, this));
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::save_to_file(const std::string_view _resource_name)
	{
		const std::shared_ptr<ResourceTypes>& savedFile = find(_resource_name);
		if (nullptr == savedFile)
		{
			return eResult::Fail_Nullptr;
		}
		
		return savedFile->save_to_file(m_BaseDir, _resource_name);
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::save_to_file(ResourceTypes* _resPtr)
	{
		if (nullptr == _resPtr)
		{
			return eResult::Fail_Nullptr;
		}
		else if (_resPtr->get_resource_name().empty())
		{
			return eResult::Fail_InValid;
		}

		return _resPtr->save_to_file(m_BaseDir, _resPtr->get_resource_name());
	}

	template<typename ResourceTypes>
	inline eResult ResourceManager<ResourceTypes>::save_to_file(ResourceTypes* _resPtr, const std::fs::path& _resource_name)
	{
		if (nullptr == _resPtr)
		{
			return eResult::Fail_Nullptr;
		}
		else if (_resource_name.empty())
		{
			return eResult::Fail_InValid;
		}

		//기존 키값 임시 저장
		std::string tempStr{ _resPtr->get_resource_name() };
		_resPtr->set_resource_name(_resource_name.string());

		//저장하고
		eResult result = _resPtr->save_to_file(m_BaseDir, _resource_name);

		//원래 키값을 복원
		_resPtr->set_resource_name(tempStr);

		return result;
	}

	template<typename ResourceTypes>
	inline void ResourceManager<ResourceTypes>::release()
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
	inline void ResourceManager<ResourceTypes>::SetBaseDir(const std::fs::path& _base_directory)
	{
		m_BaseDir = _base_directory;

		if (false == m_BaseDir.empty() && false == std::fs::exists(m_BaseDir)) {
			bool result = std::fs::create_directories(m_BaseDir);
			ASSERT(result, "리소스 기본 디렉토리 생성에 실패했습니다.");
		}
	}









}


