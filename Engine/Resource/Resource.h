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

		//리소스 찾기용 Key 역할 (파일로부터 로딩될 경우 경로를 사용.)
		const std::string& get_resource_key() const { return m_resource_key; }

		eResourceType get_resource_type() const { return m_res_type; }

	private:
		void set_resource_key(const std::string_view _resource_name) { m_resource_key = _resource_name; }
		virtual eResourceType get_resource_type_internal() const { return eResourceType::UNKNOWN; }

		eResourceType m_res_type;
		std::string m_resource_key;
	};
}
