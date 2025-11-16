#pragma once
#include <Engine/define_Macro.h>
#include <Engine/Util/Singleton.h>

#include <Engine/Resource/Resource.h>


namespace core
{
	template <typename T>
	concept ResourceType = requires {
		std::is_base_of_v<Resource, T>;
		// 1. Check if the function exists and returns bool
		//    This also implicitly checks if it's callable at compile-time,
		//    which means it should be constexpr if its result is used in 'requires'.
		{ T::template is_base_resource<T>() } -> std::same_as<bool>;

		// 2. Check if the function's return value evaluates to true at compile time.
		//    This is a nested 'requires' clause.
			requires T::template is_base_resource<T>();
	};

	class ResourceManager
	{
		friend class GameEngine;

		DECLARE_SINGLETON(ResourceManager);
	private:
		ResourceManager();
		void init();

		~ResourceManager();

	public:
		s_ptr<Resource> find(eResourceType _type, const std::string_view _resource_name);
		template <typename T>
		s_ptr<T> find(const std::string_view _resource_name)
		{
			s_ptr<Resource> result = find(T::s_resource_type, _resource_name);

			if constexpr (T::template is_base_resource<T>())
			{
				return std::static_pointer_cast<T>(result);
			}
			else
			{
				return std::dynamic_pointer_cast<T>(result);
			}
		}

		s_ptr<Resource> load(const std::string_view _class_name, const std::string_view _resource_name);
		template <typename T>
		s_ptr<T> load(const std::string_view _resource_name)
		{
			static_assert(std::is_base_of_v<Resource, T>, "Resource를 상속받은 클래스만 호출 가능");

			s_ptr<T> ret = find<T>(_resource_name);
			if (ret) { return ret; }

			ret = new_entity<T>();
			ret->set_resource_path(_resource_name);
			eResult result = load_internal(std::static_pointer_cast<Resource>(ret));
			if (eResult_fail(result)) { return nullptr; }

			return ret;
		}
		
		s_ptr<Entity> load_from_json(const std::fs::path& _json_path);

		eResult save(const s_ptr<Resource>& _res);
		eResult save_as(const s_ptr<Resource>& _res, const std::string_view _res_path);

		eResult insert(const s_ptr<Resource>& _res);

		void clean_unused();

		const std::fs::path& get_resource_directory() const { return m_resource_directory; }
		const std::fs::path& get_shader_directory() const { return m_shader_directory; }

		std::fs::path create_resource_name_from_full_path(const std::fs::path& _fullPath);

		const umap_string<s_ptr<Resource>>& get_resources(eResourceType _res_type) {
			return m_resources[(int)_res_type];
		}
		
	private:
		//중복 검사는 하지 않음. 반드시 중복 검사를 하고 호출할것.
		eResult load_internal(const s_ptr<Resource>& _res);

		void load_default_resources();

		void create_default_buffers();
		void load_default_shaders();
		void load_default_textures();
		void load_default_meshes();
		void load_default_materials();

		std::array<umap_string<s_ptr<Resource>>, (int)eResourceType::END> m_resources;

		std::fs::path m_resource_directory;
		std::fs::path m_shader_directory;
	};
}
