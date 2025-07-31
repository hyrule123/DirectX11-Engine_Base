#pragma once
#include <Engine/Game/Entity.h>

#include <Engine/Resource/define_Resource.h>

template <typename T>
struct IsBaseResource {
	constexpr static inline const bool is = false;
};

#define BASE_RESOURCE(_class) \
class _class;\
template <>\
struct IsBaseResource<_class> {\
	constexpr static inline const bool is = true;\
}

namespace core
{
	class Resource 
		: public Entity
	{
		CLASS_INFO(Resource, Entity);
	public:
		Resource();
		Resource(const Resource& _other) = default;

		virtual ~Resource();

		void get_res_filename(const std::string_view _resource_name) { m_res_filename = _resource_name; }
		const std::string& get_res_filename() const { return m_res_filename; }

		//baseDir = 해당 리소스 모음 폴더까지의 경로. ex: ....Res/Texture
		//strKeyPath = 이후 경로. ex: Player/Player.png -> 리소스를 찾는 키값으로 사용됨
		virtual eResult save(const std::fs::path& _base_directory) const;
		virtual eResult load(const std::fs::path& _base_directory);

		void set_engine_default_res(bool _bIsDefault) { m_b_engine_default_resource = _bIsDefault; }
		bool is_engine_default_res() const { return m_b_engine_default_resource; }

	private:
		std::string m_res_filename;
		bool m_b_engine_default_resource;
	};

}
