#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/Resource/define_Resource.h>

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

	public:
		Resource();
		virtual void init() override;

		Resource(const Resource& _other) = default;

		virtual ~Resource();

		void set_resource_path(const std::string_view _resource_name) { m_resource_path = _resource_name; }
		const std::string& get_resource_path() const { return m_resource_path; }

		//baseDir = 해당 리소스 모음 폴더까지의 경로. ex: ....Res/Texture
		//strKeyPath = 이후 경로. ex: Player/Player.png -> 리소스를 찾는 키값으로 사용됨
		virtual eResult save(const std::fs::path& _base_directory) const;
		virtual eResult load(const std::fs::path& _base_directory);

		void set_engine_default_res(bool _bIsDefault) { m_b_engine_default_resource = _bIsDefault; }
		bool is_engine_default_res() const { return m_b_engine_default_resource; }
		eResourceType get_resource_type() const { return m_res_type; }

	private:
		virtual eResourceType get_resource_type_internal() const { return eResourceType::UNKNOWN; }

		eResourceType m_res_type;
		bool m_b_engine_default_resource;
		std::string m_resource_path;
	};
}
