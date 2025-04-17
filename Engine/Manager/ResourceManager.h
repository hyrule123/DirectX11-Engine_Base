#pragma once
#include "Engine/Util/AtExit.h"
#include "Engine/Common.h"
#include "Engine/Resource/Resource.h"
#include "Engine/Util/StaticSingleton.h"

#include "ResourceManagers.h"

#include <concepts>
#include <mutex>

#define LOAD_COMPUTESHADER(_type) ResourceManager<ComputeShader>::get_inst().load<_type>(#_type)

namespace core
{
	template <typename T>
	concept ResourceType =
		std::is_base_of_v<Resource, T>
		&&
		IsBaseResource<T>::is;

	template <ResourceType T> 
	class ResourceManager : public StaticSingleton<ResourceManager<T>>
	{
		friend class StaticSingleton<ResourceManager<T>>;
	public:
		using Resources = 
			std::unordered_map<std::string, std::shared_ptr<T>, Hasher_StringView, std::equal_to<>>;

		void init(const std::fs::path& _base_directory);
		void release();

		//Resource Manager에 등록된 경로에 파일을 저장한다.
		eResult save(const std::shared_ptr<T>& _res);
		eResult save_as(const std::shared_ptr<T>& _res, const std::fs::path& _save_as_resource_name);

		//Resource Manager에 등록된 경로로부터 불러온다.
		template <typename Derived = T>
		std::shared_ptr<Derived> load(const std::string_view _resource_name);

		std::shared_ptr<T> load(const std::string_view _resource_name, const std::string_view _concrete_class_name);

		template <typename Derived = T>
		void insert(const std::string_view _resource_name, std::shared_ptr<Derived> _Res);

		template <typename Derived = T>
		std::shared_ptr<Derived> find(const std::string_view _resource_name);

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
		std::mutex m_mtx;

		Resources m_Resources;
	};

	template<ResourceType T>
	template<typename Derived>
	inline std::shared_ptr<Derived> ResourceManager<T>::load(const std::string_view _resource_name)
	{
		static_assert(std::is_base_of_v<T, Derived>);

		std::shared_ptr<Derived> ret = nullptr;

		if (_resource_name.empty())
		{
			return ret;
		}

		ret = find<Derived>(_resource_name);

		if(nullptr == ret)
		{
			ret = std::make_shared<Derived>();
			eResult result = result = ret->load(m_BaseDir, _resource_name);

			if (eResult_success(result))
			{
				insert(_resource_name, std::static_pointer_cast<T>(ret));
			}
			else
			{
				ret = nullptr;
			}
		}

		return ret;
	}



	template<ResourceType T>
	template<typename Derived>
	inline std::shared_ptr<Derived> ResourceManager<T>::find(const std::string_view _resource_name)
	{
		std::shared_ptr<Derived> returnRes = nullptr;

		const auto& iter = m_Resources.find(_resource_name);

		if (m_Resources.end() != iter)
		{
			if constexpr (std::is_same_v<T, Derived>) {
				returnRes = iter->second;
			}
			else {
				returnRes = std::dynamic_pointer_cast<Derived>(iter->second);
			}
		}

		return returnRes;
	}

	template<ResourceType T>
	template <typename Derived>
	inline void ResourceManager<T>::insert(const std::string_view _resource_name, std::shared_ptr<Derived> _Res)
	{
		static_assert(std::is_base_of_v<T, Derived>, "넣으려는 리소스 타입이 ResourceTypes의 상속 클래스가 아닙니다.");
		ASSERT(nullptr == find(_resource_name), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->set_resource_name(_resource_name);
		std::lock_guard<std::mutex> locker(m_mtx);
		m_Resources.insert(std::make_pair(std::string(_resource_name), std::static_pointer_cast<T>(_Res)));
	}

	template<ResourceType T>
	inline ResourceManager<T>::ResourceManager()
		: m_bInitialized{false}
		, m_BaseDir{}
		, m_Resources{}
	{
	}

	template<ResourceType T>
	inline ResourceManager<T>::~ResourceManager()
	{
	}

	template<ResourceType T>
	inline void ResourceManager<T>::init(const std::fs::path& _base_directory)
	{
		m_bInitialized = true;

		SetBaseDir(_base_directory);

		ResourceManagers::get_inst().AddUnusedResourceCleanFunc(std::bind(&ResourceManager<T>::CleanUnusedResources, this));

		AtExit::AddFunc(std::bind(&ResourceManager<T>::release, this));
	}

	template<ResourceType T>
	inline void ResourceManager<T>::release()
	{
		m_bInitialized = false;
		m_BaseDir.clear();
		m_Resources.clear();
	}

	template<ResourceType T>
	inline eResult ResourceManager<T>::save(const std::shared_ptr<T>& _res)
	{
		if (nullptr == _res || _res->get_resource_name().empty()) {
			return eResult::Fail;
		}

		return _res->save(m_BaseDir, _res->get_resource_name());
	}

	template<ResourceType T>
	inline eResult ResourceManager<T>::save_as(const std::shared_ptr<T>& _res, const std::fs::path& _save_as_resource_name)
	{
		if (nullptr == _res || _save_as_resource_name.empty()) {
			return eResult::Fail;
		}

		return _res->save(m_BaseDir, _save_as_resource_name);
	}

	template<ResourceType T>
	inline std::shared_ptr<T> ResourceManager<T>::load(const std::string_view _resource_name, const std::string_view _concrete_class_name)
	{
		std::shared_ptr<T> ret = find<T>(_resource_name);

		//없을 경우 클래스 이름을 통해 새로 생성
		if (nullptr == ret) {
			ret = std::shared_ptr<T>(EntityFactory::get_inst().instantiate<T>(_concrete_class_name).release());

			if (ret && eResult_success(ret->load(m_BaseDir, _resource_name))) {
				ret->set_resource_name(_resource_name);
			}
			else {
				ret = nullptr;
			}
		}

		return ret;
	}

	template<ResourceType T>
	inline std::vector<std::shared_ptr<Resource>> ResourceManager<T>::GetResourcesVector()
	{
		std::vector<std::shared_ptr<Resource>> retVec{};

		for (const auto& iter : m_Resources)
		{
			retVec.emplace_back(std::static_pointer_cast<Resource>(iter.second));
		}

		return retVec;
	}

	template<ResourceType T>
	inline void ResourceManager<T>::CleanUnusedResources()
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

	template<ResourceType T>
	inline void ResourceManager<T>::SetBaseDir(const std::fs::path& _base_directory)
	{
		m_BaseDir = _base_directory;

		if (false == m_BaseDir.empty() && false == std::fs::exists(m_BaseDir)) {
			bool result = std::fs::create_directories(m_BaseDir);
			ASSERT(result, "리소스 기본 디렉토리 생성에 실패했습니다.");
		}
	}









}


