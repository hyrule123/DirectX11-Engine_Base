#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/Resource/define_Resource.h>

//ResourceManager에 의해 관리받는 클래스
//반드시 ResourceManager을 통해서 생성해야 함.

namespace core
{
#define BASE_RESOURCE(_class, _eResourceType) \
public:\
	constexpr static const eResourceType s_resource_type = _eResourceType;\
	template <typename T>\
	constexpr static bool is_base_resource() { return std::is_same_v<_class, T>; }\
protected:\
	virtual eResourceType get_resource_type_internal() const override { return s_resource_type; }\

	class Resource 
		: public Entity
	{
		CLASS_INFO(Resource, Entity);

		friend class ResourceManager;

	public:
		Resource();
		virtual void init() override;

		Resource(const Resource& _other) = default;

		virtual ~Resource();

		template <typename T>
		constexpr static bool is_base_resource() { return std::is_same_v<Resource, T>; }

		virtual eResult save(const std::fs::path& _fullpath) const = 0;
		virtual eResult load(const std::fs::path& _fullpath) = 0;

		//경로 겸 리소스 찾기용 Key 역할
		void set_res_pathkey(const std::string_view _resource_name) { m_res_pathkey = _resource_name; }
		const std::string& get_res_pathkey() const { return m_res_pathkey; }

		eResourceType get_resource_type() const { return m_res_type; }

	private:
		virtual eResourceType get_resource_type_internal() const { return eResourceType::UNKNOWN; }

		eResourceType m_res_type;
		std::string m_res_pathkey;
	};
}
