#pragma once
#include "Engine/Entity.h"

#include "Engine/Resource/define_Resource.h"

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

namespace ehw
{
	class Resource 
		: public Entity
	{
	public:
		Resource(const std::string_view _concrete_class_name);
		Resource(const Resource& _other) = default;

		virtual ~Resource();

		void set_keypath(const std::string_view _key_path) { m_path = _key_path; }
		const std::string_view get_path() const { return m_path; }

		//baseDir = 해당 리소스 모음 폴더까지의 경로. ex: ....Res/Texture
		//strKeyPath = 이후 경로. ex: Player/Player.png -> 리소스를 찾는 키값으로 사용됨
		virtual eResult save(const std::fs::path& _base_directory, const std::fs::path& _key_path) const;
		virtual eResult load(const std::fs::path& _base_directory, const std::fs::path& _key_path) = 0;

		void set_engine_default_res(bool _bIsDefault) { mbEngineDefaultRes = _bIsDefault; }
		bool IsEngineDefaultRes() const { return mbEngineDefaultRes; }

	private:
		std::string m_path;
		bool mbEngineDefaultRes;
	};

}
