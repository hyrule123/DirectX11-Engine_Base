#pragma once
#include <Engine/Util/AtExit.h>
#include <Engine/Common.h>
#include <Engine/Resource/Resource.h>
#include <Engine/Util/Singleton.h>

#include <Engine/Manager/ResourceManagers.h>

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
	class ResourceManager
	{
		DECLARE_SINGLETON(ResourceManager<T>);
	private:
		ResourceManager() : m_b_initialized(), m_base_directory(), m_mtx(), m_resources() {}
		void init()
		{
			m_b_initialized = false;

			AtExit::add_func(ResourceManager<T>::destroy_inst);
		}
		~ResourceManager();
		
	public:
		using Resources = 
			std::unordered_map<std::string, s_ptr<T>, Hasher_StringView, std::equal_to<>>;

		void init(const std::fs::path& _base_directory);
		void release();

		//Resource Manager에 등록된 경로에 파일을 저장한다.
		eResult save(const s_ptr<T>& _res);
		eResult save_as(const s_ptr<T>& _res, const std::fs::path& _save_as_resource_name);

		//Resource Manager에 등록된 경로로부터 불러온다.
		template <typename Derived = T>
		s_ptr<Derived> load(const std::string_view _resource_name);

		s_ptr<T> load(const std::string_view _resource_name, const std::string_view _static_type_name);

		template <typename Derived = T>
		void insert(const std::string_view _resource_name, s_ptr<Derived> _Res);

		template <typename Derived = T>
		s_ptr<Derived> find(const std::string_view _resource_name);

		const Resources& get_resources() { return m_resources; }

		std::vector<s_ptr<Resource>> get_resources_vector();

		void clean_unused();

		void set_base_directory(const std::fs::path& _base_directory);
		const std::fs::path& get_base_directory() { return m_base_directory; }

	private:
		bool m_b_initialized;
		std::fs::path m_base_directory;
		std::mutex m_mtx;

		Resources m_resources;
	};

	template<ResourceType T>
	template<typename Derived>
	inline s_ptr<Derived> ResourceManager<T>::load(const std::string_view _resource_name)
	{
		static_assert(std::is_base_of_v<T, Derived>);

		s_ptr<Derived> ret = nullptr;

		if (_resource_name.empty())
		{
			return ret;
		}

		ret = find<Derived>(_resource_name);

		if(nullptr == ret)
		{
			ret = std::make_shared<Derived>();
			ret->get_res_filename(_resource_name);
			eResult result = result = ret->load(m_base_directory);

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
	inline s_ptr<Derived> ResourceManager<T>::find(const std::string_view _resource_name)
	{
		s_ptr<Derived> returnRes = nullptr;

		const auto& iter = m_resources.find(_resource_name);

		if (m_resources.end() != iter)
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
	inline void ResourceManager<T>::insert(const std::string_view _resource_name, s_ptr<Derived> _Res)
	{
		static_assert(std::is_base_of_v<T, Derived>, "넣으려는 리소스 타입이 ResourceTypes의 상속 클래스가 아닙니다.");
		ASSERT(nullptr == find(_resource_name), "이미 동일한 키값을 가진 리소스가 있습니다.");

		_Res->get_res_filename(_resource_name);
		std::lock_guard<std::mutex> locker(m_mtx);
		m_resources.insert(std::make_pair(std::string(_resource_name), std::static_pointer_cast<T>(_Res)));
	}

	template<ResourceType T>
	inline ResourceManager<T>::~ResourceManager()
	{
	}

	template<ResourceType T>
	inline void ResourceManager<T>::init(const std::fs::path& _base_directory)
	{
		m_b_initialized = true;

		set_base_directory(_base_directory);

		ResourceManagers::get_inst().add_clean_unused_func(std::bind(&ResourceManager<T>::clean_unused, this));

		AtExit::add_func(std::bind(&ResourceManager<T>::release, this));
	}

	template<ResourceType T>
	inline void ResourceManager<T>::release()
	{
		m_b_initialized = false;
		m_base_directory.clear();
		m_resources.clear();
	}

	template<ResourceType T>
	inline eResult ResourceManager<T>::save(const s_ptr<T>& _res)
	{
		if (nullptr == _res || _res->get_res_filename().empty()) {
			return eResult::Fail;
		}

		return _res->save(m_base_directory);
	}

	template<ResourceType T>
	inline eResult ResourceManager<T>::save_as(const s_ptr<T>& _res, const std::fs::path& _save_as_resource_name)
	{
		if (nullptr == _res || _save_as_resource_name.empty()) {
			return eResult::Fail;
		}

		//이전 이름 받아 놓는다
		std::string prev_name = _res->get_res_filename();

		//새 이름으로 교체
		_res->get_res_filename(_save_as_resource_name.string());

		//저장
		eResult result = save(_res);
		if (eResult_fail(result)) { return result; }

		//다시 기존 이름으로 변경
		_res->get_res_filename(prev_name);

		return result;
	}

	template<ResourceType T>
	inline s_ptr<T> ResourceManager<T>::load(const std::string_view _resource_name, const std::string_view _static_type_name)
	{
		s_ptr<T> ret = find<T>(_resource_name);

		//없을 경우 클래스 이름을 통해 새로 생성
		if (nullptr == ret) {
			ret = std::dynamic_pointer_cast<T>(EntityFactory::get_inst().instantiate(_static_type_name));

			if (ret && eResult_success(ret->load(m_base_directory))) {
				ret->get_res_filename(_resource_name);
			}
			else {
				ret = nullptr;
			}
		}

		return ret;
	}

	template<ResourceType T>
	inline std::vector<s_ptr<Resource>> ResourceManager<T>::get_resources_vector()
	{
		std::vector<s_ptr<Resource>> retVec{};

		for (const auto& iter : m_resources)
		{
			retVec.emplace_back(std::static_pointer_cast<Resource>(iter.second));
		}

		return retVec;
	}

	template<ResourceType T>
	inline void ResourceManager<T>::clean_unused()
	{
		using pairType = decltype(m_resources)::value_type;
		std::erase_if(m_resources,
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
	inline void ResourceManager<T>::set_base_directory(const std::fs::path& _base_directory)
	{
		m_base_directory = _base_directory;

		if (false == m_base_directory.empty() && false == std::fs::exists(m_base_directory)) {
			bool result = std::fs::create_directories(m_base_directory);
			ASSERT(result, "리소스 기본 디렉토리 생성에 실패했습니다.");
		}
	}









}


